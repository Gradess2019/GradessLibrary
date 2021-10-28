// Copyright Gradess. All Rights Reserved.


#include "Components/GLibVisionFieldComponent.h"

#include "ProceduralMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
	SCOPE_LOG_TIME_FUNC();

	TArray<FHitResult> Hits;
	Execute(Hits);
}

void UGLibVisionFieldComponent::InitializeComponent()
{
	Super::InitializeComponent();

	DebugMesh = NewObject<UProceduralMeshComponent>(GetOwner());
	DebugMesh->RegisterComponent();
	DebugMesh->AttachToComponent(
		GetOwner()->GetRootComponent(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale
	);

	DebugMesh->CastShadow = false;
}

void UGLibVisionFieldComponent::Execute(TArray<FHitResult>& OutHits)
{
	Setup();

	TArray<FHitResult> Hits;
	LaunchLineTraces(Hits);

	TArray<FHitResult> PrecisedHits;
	PreciseObjectBorders(Hits, PrecisedHits);

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
	
	DebugMesh->ClearAllMeshSections();
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

	do
	{
		auto CurrentAngle = PreciseAngle;
		auto Count = 0;

		while (PreciseAngle <= CurrentAngle && Count < MaxPreciseCount)
		{
			Count++;

			auto LeftRotation = UKismetMathLibrary::MakeRotFromX(LeftDirection);
			auto RightRotation = UKismetMathLibrary::MakeRotFromX(RightDirection);

			CurrentAngle = FMath::Abs((LeftRotation.Yaw - RightRotation.Yaw) / 2.f);

			auto NormalizedLeftDirection = LeftDirection;
			auto NormalizedRightDirection = RightDirection;
			NormalizedLeftDirection.Normalize();
			NormalizedRightDirection.Normalize();

			// TODO: can we just divide it by 2?
			auto MidDirection = (LeftDirection + RightDirection) / 2.f;
			// MidDirection.Normalize();
			auto MidEndPoint = MidDirection * TraceDistance + StartPoint;

			FHitResult Hit;
			LaunchTrace(StartPoint, MidEndPoint, Hit);

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
			LeftHit,
			DrawDebugType
		);

		FHitResult RightHit;
		LaunchTrace(
			StartPoint,
			StartPoint + RightDirection * TraceDistance,
			RightHit,
			DrawDebugType
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
