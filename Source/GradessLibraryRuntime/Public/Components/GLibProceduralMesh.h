// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GLibProceduralMesh.generated.h"

USTRUCT(
	BlueprintType
)
struct FGeneratedWallData
{
	GENERATED_BODY()

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|ProcWallData"
	)
	TArray<FVector> Vertices;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|ProcWallData"
	)
	TArray<int32> Triangles;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|ProcWallData"
	)
	TArray<FVector> Normals;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|ProcWallData"
	)
	TArray<FVector2D> UVs;

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|ProcWallData"
	)
	TArray<FProcMeshTangent> Tangents;

	FGeneratedWallData()
		: Vertices({}),
		  Triangles({}),
		  Normals({}),
		  UVs({}),
		  Tangents({})
	{
	}

	FGeneratedWallData(
		const TArray<FVector>& Vertices,
		const TArray<int32>& Triangles,
		const TArray<FVector>& Normals,
		const TArray<FVector2D>& UVs,
		const TArray<FProcMeshTangent>& Tangents
	)
		: Vertices(Vertices),
		  Triangles(Triangles),
		  Normals(Normals),
		  UVs(UVs),
		  Tangents(Tangents)
	{
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRADESSLIBRARYRUNTIME_API UGLibProceduralMesh : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UGLibProceduralMesh(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(
		BlueprintCallable,
		meta = (
			AutoCreateRefTerm = "StartLocation, EndLocation"
		)
	)
	void CreatePlaneWall(
		const FVector& StartLocation,
		const FVector& EndLocation,
		const float Height,
		FGeneratedWallData& Data
	);

	UFUNCTION(
		BlueprintCallable,
		meta = (
			AutoCreateRefTerm = "StartLocation, EndLocation"
		)
	)
	void CreatePlaneWallOnGrid(
		const FVector& StartLocation,
		const FVector& EndLocation,
		const float Height,
		const float GridSize,
		FGeneratedWallData& Data
	);
};
