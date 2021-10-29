// Copyright Gradess. All Rights Reserved.


#include "Components/GLibVisionFieldComponent.h"

#include "ProceduralMeshComponent.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UGLibVisionFieldComponent::UGLibVisionFieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	TraceColor = FColor::Red;
	TraceHitColor = FColor::Green;
	DebugDrawTime = 3.f;
	DrawDebugType = EDrawDebugTrace::None;

	FieldOfView = 90.f;
	TraceDistance = 1500.f;

	SectionsNumber = 10;

	PreciseAngle = 5.f;
	MaxPreciseCount = 20;
}

void UGLibVisionFieldComponent::TickComponent(
	float DeltaTime,
	ELevelTick Tick,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, Tick, ThisTickFunction);

	UKismetMaterialLibrary::SetVectorParameterValue(
		this,
		RenderData,
		"PlayerLocation",
		GetOwner()->GetActorLocation()
	);

	UKismetMaterialLibrary::SetScalarParameterValue(
		this,
		RenderData,
		"Size",
		TraceDistance * 2.f
	);

	TArray<FHitResult> Hits;
	Execute(Hits);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget);

	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);

	TArray<FCanvasUVTri> Triangles;

	for (auto& Hit : Hits)
	{
		auto Direction = Hit.TraceEnd - Hit.TraceEnd;

		Direction.Normalize();

		Hit.TraceEnd = Hit.TraceEnd + Direction * 10.f;
	}

	const auto Center = Size / 2.f;
	const auto TextureBorderMargin = TraceDistance * 0.1;
	const auto VectorToTextureSpaceScale = Center.X / (TraceDistance + TextureBorderMargin);
	const auto ActorLocation = FVector2D(GetOwner()->GetActorLocation()) * VectorToTextureSpaceScale;
	const auto Color = FColor(255, 255, 255, 0.f);
	for (int32 Id = 0; Id < Hits.Num() - 1; Id++)
	{
		FCanvasUVTri NewTriangle;

		const auto FirstHit = Hits[Id];
		const auto SecondHit = Hits[Id + 1];

		const auto FirstHitLocation =
			FirstHit.bBlockingHit ? FVector2D(FirstHit.Location) : FVector2D(FirstHit.TraceEnd);
		const auto SecondHitLocation =
			SecondHit.bBlockingHit ? FVector2D(SecondHit.Location) : FVector2D(SecondHit.TraceEnd);

		NewTriangle.V0_Pos = Center;
		NewTriangle.V1_Pos = FirstHitLocation * VectorToTextureSpaceScale - ActorLocation + Center;
		NewTriangle.V2_Pos = SecondHitLocation * VectorToTextureSpaceScale - ActorLocation + Center;

		NewTriangle.V0_Color = Color;
		NewTriangle.V1_Color = Color;
		NewTriangle.V2_Color = Color;

		Triangles.Add(NewTriangle);
	}
	Canvas->K2_DrawTriangle(nullptr, Triangles);
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);

	bSnapshot = false;
}

void UGLibVisionFieldComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (DrawDebugType != EDrawDebugTrace::None)
	{
		DebugMesh = NewObject<UProceduralMeshComponent>(GetOwner());
		DebugMesh->RegisterComponent();
		DebugMesh->AttachToComponent(
			GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale
		);

		DebugMesh->CastShadow = false;
	}
}

void UGLibVisionFieldComponent::Execute(TArray<FHitResult>& OutHits)
{
	Setup();

	TArray<FHitResult> Hits;
	LaunchLineTraces(Hits);

	TArray<FHitResult> PrecisedHits;
	PreciseObjectBorders(Hits, PrecisedHits);

	if (DebugMesh)
	{
		DebugMesh->ClearAllMeshSections();

		const auto StartPoint = GetOwner()->GetActorLocation();
		for (int32 Id = 0; Id < PrecisedHits.Num() - 1; Id++)
		{
			auto& FirstHit = PrecisedHits[Id];
			auto& SecondHit = PrecisedHits[Id + 1];

			auto LeftPoint = FirstHit.bBlockingHit ? FirstHit.Location : FirstHit.TraceEnd;
			auto RightPoint = SecondHit.bBlockingHit ? SecondHit.Location : SecondHit.TraceEnd;

			DebugMesh->CreateMeshSection(
				Id,
				TArray<FVector>({FVector::ZeroVector, LeftPoint - StartPoint, RightPoint - StartPoint}),
				TArray<int32>({0, 2, 1}),
				TArray<FVector>(),
				TArray<FVector2D>(),
				TArray<FColor>(),
				TArray<FProcMeshTangent>(),
				false
			);
		}

		const auto DebugMeshRotation = UKismetMathLibrary::MakeRotFromX(GetOwner()->GetActorForwardVector()) * -1;
		DebugMesh->SetRelativeRotation(DebugMeshRotation);

		if (DebugMeshMaterial)
		{
			for (int32 Id = 0; Id < PrecisedHits.Num(); Id++)
			{
				DebugMesh->SetMaterial(Id, DebugMeshMaterial);
			}
		}
	}
	OutHits = PrecisedHits;
}

float UGLibVisionFieldComponent::GetAnglePerTrace() const
{
	return AnglePerTrace;
}

UProceduralMeshComponent* UGLibVisionFieldComponent::GetDebugMesh() const
{
	return DebugMesh;
}

void UGLibVisionFieldComponent::Setup()
{
	AnglePerTrace = FieldOfView / SectionsNumber;
}

void UGLibVisionFieldComponent::LaunchLineTraces(TArray<FHitResult>& Hits)
{
	Hits.Reset(SectionsNumber);
	const auto Forward = GetOwner()->GetActorForwardVector();
	const auto ActorLocation = GetOwner()->GetActorLocation();

	const auto HalfAngle = FieldOfView / 2.f;
	for (int32 Id = 0; Id <= SectionsNumber; Id++)
	{
		auto CurrentAngle = Id * AnglePerTrace - HalfAngle;
		auto TraceDirection = Forward.RotateAngleAxis(CurrentAngle, FVector::UpVector);
		auto EndLocation = TraceDirection * TraceDistance + ActorLocation;

		FHitResult Hit;
		UKismetSystemLibrary::LineTraceSingle(
			this,
			ActorLocation,
			EndLocation,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			TArray<AActor*>(),
			DrawDebugType,
			Hit,
			true,
			TraceColor,
			TraceHitColor,
			DebugDrawTime
		);

		Hits.Add(Hit);
	}
}

void UGLibVisionFieldComponent::PreciseObjectBorders(
	const TArray<FHitResult>& Hits,
	TArray<FHitResult>& PrecisedHits
)
{
	PrecisedHits.Reset(Hits.Num() * 2);

	const auto StartPoint = GetOwner()->GetActorLocation();

	for (int32 Id = 0; Id < Hits.Num() - 1; Id++)
	{
		auto& FirstHit = Hits[Id];
		auto& SecondHit = Hits[Id + 1];

		PrecisedHits.Add(FirstHit);

		if (FirstHit.GetActor() != SecondHit.GetActor())
		{
			auto LeftPoint = FirstHit.bBlockingHit ? FirstHit.Location : FirstHit.TraceEnd;
			auto RightPoint = SecondHit.bBlockingHit ? SecondHit.Location : SecondHit.TraceEnd;

			TArray<FHitResult> NewHits;
			ExecuteBisectionMethod(
				StartPoint,
				LeftPoint,
				RightPoint,
				FirstHit.GetActor(),
				SecondHit.GetActor(),
				NewHits
			);

			PrecisedHits.Append(NewHits);
		}
	}

	PrecisedHits.Add(Hits.Last());
}

void UGLibVisionFieldComponent::ExecuteBisectionMethod(
	const FVector& StartPoint,
	const FVector& LeftPoint,
	const FVector& RightPoint,
	const AActor* LeftActor,
	const AActor* RightActor,
	TArray<FHitResult>& PrecisedHits
)
{
	auto LeftDirection = LeftPoint - StartPoint;
	auto RightDirection = RightPoint - StartPoint;

	TArray<AActor*> MidActors;
	int32 DebugCount = 0;
	do
	{
		auto CurrentAngle = PreciseAngle;
		auto Count = 0;

		while (PreciseAngle <= CurrentAngle && Count < MaxPreciseCount)
		{
			DebugCount++;
			Count++;

			auto LeftRotation = UKismetMathLibrary::MakeRotFromX(LeftDirection);
			auto RightRotation = UKismetMathLibrary::MakeRotFromX(RightDirection);

			CurrentAngle = FMath::Abs((LeftRotation.Yaw - RightRotation.Yaw) / 2.f);

			auto NormalizedLeftDirection = LeftDirection;
			auto NormalizedRightDirection = RightDirection;
			NormalizedLeftDirection.Normalize();
			NormalizedRightDirection.Normalize();

			auto MidDirection = NormalizedLeftDirection + NormalizedRightDirection;
			MidDirection.Normalize();
			auto MidEndPoint = MidDirection * TraceDistance;

			FHitResult Hit;
			LaunchTrace(StartPoint, StartPoint + MidEndPoint, Hit);

			if (DebugCount == CountToDraw)
			{
				FHitResult HitAAAA;
				LaunchTrace(StartPoint, StartPoint + NormalizedLeftDirection * TraceDistance, HitAAAA,  DrawDebugType, FColor::White);
				LaunchTrace(StartPoint, StartPoint + NormalizedRightDirection * TraceDistance, HitAAAA,  DrawDebugType, FColor::White);
				LaunchTrace(StartPoint, StartPoint + MidEndPoint, HitAAAA,  DrawDebugType, FColor::Blue);
			}

			if (bSnapshot)
			{
				TestHits.Add(Hit);
			}
			
			if (LeftActor == Hit.GetActor())
			{
				LeftDirection = MidDirection;
			}
			else if (RightActor == Hit.GetActor())
			{
				RightDirection = MidDirection;
			}
			else
			{
				MidActors.Insert(Hit.GetActor(), 0);
			}
		}

		FHitResult LeftHit;
		LaunchTrace(
			StartPoint,
			StartPoint + LeftDirection * TraceDistance,
			LeftHit
			// DrawDebugType
		);

		FHitResult RightHit;
		LaunchTrace(
			StartPoint,
			StartPoint + RightDirection * TraceDistance,
			RightHit
			// DrawDebugType
		);

		PrecisedHits.Add(LeftHit);
		PrecisedHits.Add(RightHit);

		if (MidActors.Num() <= 0) { return; }

		LeftActor = MidActors[0];
		MidActors.RemoveAt(0);
		LeftDirection = RightDirection;
		RightDirection = RightPoint - StartPoint;
	}
	while (MidActors.Num() > 0);
}

void UGLibVisionFieldComponent::LaunchTrace(
	const FVector& StartPoint,
	const FVector& MidEndPoint,
	FHitResult& Hit,
	TEnumAsByte<EDrawDebugTrace::Type> DebugType,
	const FColor& CustomTraceColor,
	const FColor& CustomTraceHitColor,
	float CustomDrawTime
)
{
	UKismetSystemLibrary::LineTraceSingle(
		this,
		StartPoint,
		MidEndPoint,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),
		DebugType,
		Hit,
		true,
		CustomTraceColor,
		CustomTraceHitColor,
		CustomDrawTime
	);
}
