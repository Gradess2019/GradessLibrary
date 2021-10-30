// Copyright Gradess. All Rights Reserved.


#include "Components/GLibVisionFieldComponent.h"

#include "ProceduralMeshComponent.h"
#include "Core/GLibMathLibrary.h"
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
	// SCOPE_LOG_TIME_FUNC();
	
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
	TArray<FHitResult> Hits,
	TArray<FHitResult>& PrecisedHits
)
{
	PrecisedHits.Reset(Hits.Num() * 2);

	const auto StartPoint = GetOwner()->GetActorLocation();

	for (int32 Id = 0; Id < Hits.Num() - 1 && Id < 100; Id++)
	{
		auto& LeftHit = Hits[Id];
		auto& RightHit = Hits[Id + 1];
		
		if (!LeftHit.Normal.Equals(RightHit.Normal) || LeftHit.GetActor() != RightHit.GetActor())
		{
			TArray<FHitResult> NewHits;
			ExecuteBisectionMethodAdvanced(
				StartPoint,
				LeftHit,
				RightHit,
				NewHits,
				Hits,
				Id
			);

			PrecisedHits.Add(LeftHit);
			PrecisedHits.Append(NewHits);
		} else
		{
			PrecisedHits.Add(LeftHit);
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
				LaunchTrace(StartPoint, StartPoint + NormalizedLeftDirection * TraceDistance, HitAAAA, DrawDebugType,
				            FColor::White);
				LaunchTrace(StartPoint, StartPoint + NormalizedRightDirection * TraceDistance, HitAAAA, DrawDebugType,
				            FColor::White);
				LaunchTrace(StartPoint, StartPoint + MidEndPoint, HitAAAA, DrawDebugType, FColor::Blue);
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

void UGLibVisionFieldComponent::ExecuteBisectionMethodAdvanced(
	const FVector& StartLocation,
	const FHitResult& LeftHit,
	const FHitResult& RightHit,
	TArray<FHitResult>& PrecisedHits,
	TArray<FHitResult>& HitsToInsertCorners,
	const int32 CurrentHitId
)
{
	auto LeftActor = LeftHit.GetActor();
	auto RightActor = RightHit.GetActor();

	auto LeftLocation = GetEndLocation(LeftHit);
	auto RightLocation = GetEndLocation(RightHit);

	auto LeftNormal = LeftHit.Normal;
	auto RightNormal = RightHit.Normal;

	auto MiddleHit = FHitResult();

	auto CurrentAngle = PreciseAngle;

	auto MiddleActors = TArray<AActor*>();
	
	do
	{
		auto bCorner = false;
		for (int32 Count = 0; Count < MaxPreciseCount && PreciseAngle <= CurrentAngle; Count++)
		{
			auto LeftDirection = LeftLocation - StartLocation;
			auto RightDirection = RightLocation - StartLocation;
			LeftDirection.Normalize();
			RightDirection.Normalize();

			CurrentAngle = UGLibMathLibrary::ShortestAngleBetweenVectorsInDegrees(LeftDirection, RightDirection);

			auto MiddleDirection = LeftDirection + RightDirection;
			MiddleDirection.Normalize();

			auto MiddleEndLocation = MiddleDirection * TraceDistance + StartLocation;

			LaunchTrace(StartLocation, MiddleEndLocation, MiddleHit);

			const auto& MiddleNormal = MiddleHit.Normal;

#pragma region Debug
			if (Count == CountToDraw)
			{
				auto TempHit = FHitResult();
				LaunchTrace(StartLocation, MiddleEndLocation, TempHit, EDrawDebugTrace::ForOneFrame, FColor::Blue);
				LaunchTrace(StartLocation, LeftDirection * TraceDistance + StartLocation, TempHit,
				            EDrawDebugTrace::ForOneFrame, FColor::White);
				LaunchTrace(StartLocation, RightDirection * TraceDistance + StartLocation, TempHit,
				            EDrawDebugTrace::ForOneFrame, FColor::White);
			}
#pragma endregion Debug

			if (!LeftNormal.Equals(RightNormal) && LeftActor == RightActor)
			{
				if (LeftNormal.Equals(MiddleNormal))
				{
					LeftNormal = MiddleNormal;
					LeftLocation = GetEndLocation(MiddleHit);
				}
				else
				{
					RightNormal = MiddleNormal;
					RightLocation = GetEndLocation(MiddleHit);
				}
				bCorner = true;
			}
			else if (LeftActor != RightActor)
			{
				if (LeftActor == MiddleHit.GetActor())
				{
					LeftNormal = MiddleNormal;
					LeftLocation = GetEndLocation(MiddleHit);
				}
				else if (RightActor == MiddleHit.GetActor())
				{
					RightNormal = MiddleNormal;
					RightLocation = GetEndLocation(MiddleHit);
				}
				else
				{
					MiddleActors.Insert(MiddleHit.GetActor(), 0);
				}
			}
		}
		HitsToInsertCorners.Insert(MiddleHit, CurrentHitId + 1);
		
		if (!bCorner)
		{
			auto PrecisedLeftHit = FHitResult();
			auto PrecisedRightHit = FHitResult();

			auto LeftDirection = LeftLocation - StartLocation;
			auto RightDirection = RightLocation - StartLocation;
			LeftDirection.Normalize();
			RightDirection.Normalize();

			LaunchTrace(StartLocation, LeftDirection * TraceDistance + StartLocation, PrecisedLeftHit,
			            EDrawDebugTrace::ForOneFrame, FColor::Orange);
			LaunchTrace(StartLocation, RightDirection * TraceDistance + StartLocation, PrecisedRightHit,
			            EDrawDebugTrace::ForOneFrame, FColor::Orange);

			if (!PrecisedLeftHit.bBlockingHit || !PrecisedRightHit.bBlockingHit)
			{
				MiddleHit.bBlockingHit = false;
			}
			else if (PrecisedLeftHit.Distance > PrecisedRightHit.Distance)
			{
				MiddleHit.bBlockingHit = true;
				MiddleHit.Location = PrecisedLeftHit.Location;
			} else
			{
				MiddleHit.bBlockingHit = true;
				MiddleHit.Location = PrecisedRightHit.Location;
			}
		}

		PrecisedHits.Add(MiddleHit);


		if (MiddleActors.Num() <= 0) { return; }

		LeftActor = MiddleActors[0];
		MiddleActors.RemoveAt(0);

		LeftNormal = RightNormal;
		LeftLocation = RightLocation;
	}
	while (true);
}

FVector_NetQuantize UGLibVisionFieldComponent::GetEndLocation(const FHitResult& LeftHit)
{
	return LeftHit.bBlockingHit ? LeftHit.Location : LeftHit.TraceEnd;
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
