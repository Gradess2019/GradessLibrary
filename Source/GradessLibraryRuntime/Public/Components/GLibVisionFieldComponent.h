// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "Programs/UnrealLightmass/Private/ImportExport/3DVisualizer.h"
#include "GLibVisionFieldComponent.generated.h"

#pragma region Forward declarations
class UProceduralMeshComponent;
#pragma endregion Forward declarations

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	),
	meta=(
		BlueprintSpawnableComponent
	)
)
class GRADESSLIBRARYRUNTIME_API UGLibVisionFieldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGLibVisionFieldComponent();

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

public:
	/**
	 * @brief Debug trace color
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	FLinearColor TraceColor;

	/**
	 * @brief Debug trace hit color
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	FLinearColor TraceHitColor;

	/**
	 * @brief How long trace should be displayed on the screen
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	float DebugDrawTime;

	/**
	 * @brief Custom debug procedural mesh material
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	UMaterialInterface* DebugMeshMaterial;

	/**
	 * @brief Render target to draw vision field
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	UTextureRenderTarget2D* RenderTarget;

	/**
	 * @brief Parameters that is used in postprocess material
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	UMaterialParameterCollection* RenderData;

	/**
	 * @brief Filed of view in degrees
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent",
		meta = (
			UIMin = "1",
			UIMax = "180",
			ClampMin = "1",
			ClampMax = "180"
		)
	)
	float FieldOfView;

	/**
	 * @brief Trace distance to hit objects
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	float TraceDistance;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TArray<FHitResult> TestHits;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	bool bSnapshot;
	
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	int32 CountToDraw;

	/**
	 * @brief Target minim angle that we have to get after precise execution
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	float PreciseAngle;

	/**
	 * @brief Max count of precise operations
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	int32 MaxPreciseCount;

	/**
	 * @brief Minimal number of traces 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|VisionFieldComponent"
	)
	int32 SectionsNumber;

	/**
	 * @brief Trace draw debug type
	 */
	UPROPERTY(
		EditAnywhere,
		Category = "GLib|VisionFieldComponent|Debug"
	)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

protected:
	/**
	 * @brief Calculated angle per trace
	 */
	UPROPERTY(
		BlueprintGetter = GetAnglePerTrace
	)
	float AnglePerTrace;

	/**
	 * @brief Debug procedural mesh that display "vision field" 
	 */
	UPROPERTY(
		BlueprintGetter = GetDebugMesh
	)
	UProceduralMeshComponent* DebugMesh;

public:
	virtual void InitializeComponent() override;

	/**
	 * @brief Calculates "vision field" and return precised hits
	 * @param OutHits precised result hits
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|VisionFieldComponent"
	)
	void Execute(UPARAM(DisplayName = "Hits") TArray<FHitResult>& OutHits);

	/**
	 * @brief Returns calculated angle per trace. Recalculates everytime before Execute() main work
	 * @return angle per trace value
	 */
	UFUNCTION(
		BlueprintGetter,
		Category = "GLib|VisionFieldComponent"
	)
	float GetAnglePerTrace() const;

	/**
	 * @brief Returns debug mesh that represents "vision field" 
	 * @return procedural debug mesh
	 */
	UFUNCTION(
		BlueprintGetter,
		Category = "GLib|VisionFieldComponent"
	)
	UProceduralMeshComponent* GetDebugMesh() const;

protected:
	/**
	 * @brief Prepares data before Execute() work
	 */
	UFUNCTION()
	void Setup();

	/**
	 * @brief Launches ordinary line traces in world to get base data 
	 * @param Hits result hits
	 */
	UFUNCTION()
	void LaunchLineTraces(TArray<FHitResult>& Hits);

	/**
	 * @brief Calculates geometry borders to get more precision for "vision field"
	 * @param Hits hits to precise
	 * @param PrecisedHits precised hits
	 */
	UFUNCTION()
	void PreciseObjectBorders(
		TArray<FHitResult> Hits,
		TArray<FHitResult>& PrecisedHits
	);

	UFUNCTION()
	void LaunchTrace(
		const FVector& StartPoint,
		const FVector& MidEndPoint,
		FHitResult& Hit,
		TEnumAsByte<EDrawDebugTrace::Type> DebugType = EDrawDebugTrace::None,
		const FColor& CustomTraceColor = FColor::Blue,
		const FColor& CustomTraceHitColor = FColor::Cyan,
		float CustomDrawTime = 5.f
	);

	/**
	 * @brief Calculates precised hits for geometry using bisection method
	 * @param StartPoint bisection start point. Usually owner world location
	 * @param LeftPoint left point world location
	 * @param RightPoint right point world location
	 * @param LeftActor actor to check during bisection
	 * @param RightActor actor to check during bisecting
	 * @param PrecisedHits result precised hits
	 */
	UFUNCTION()
	void ExecuteBisectionMethod(
		const FVector& StartPoint,
		const FVector& LeftPoint,
		const FVector& RightPoint,
		const AActor* LeftActor,
		const AActor* RightActor,
		TArray<FHitResult>& PrecisedHits
	);
	FVector_NetQuantize GetEndLocation(const FHitResult& LeftHit);

	void ExecuteBisectionMethodAdvanced(
		const FVector& StartLocation,
		const FHitResult& LeftHit,
		const FHitResult& RightHit,
		TArray<FHitResult>& PrecisedHits,
		FHitResult*& Corner
	);
};
