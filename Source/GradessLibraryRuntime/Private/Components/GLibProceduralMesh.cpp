// Copyright Gradess. All Rights Reserved.


#include "Components/GLibProceduralMesh.h"

#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UGLibProceduralMesh::UGLibProceduralMesh(const FObjectInitializer& ObjectInitializer)
	: UProceduralMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UGLibProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
}

void UGLibProceduralMesh::CreatePlaneWall(
	const FVector& StartLocation,
	const FVector& EndLocation,
	const float Height,
	FGeneratedWallData& Data
)
{
	const auto Direction = EndLocation - StartLocation;
	const auto Length = Direction.Size();

	const auto Rotation = Direction.Rotation();

	Data.Vertices = TArray<FVector>({
		{0, 0, 0},
		{Length, 0, 0},
		{0, 0, Height},
		{Length, 0, Height}
	});

	for (auto& Vertex : Data.Vertices)
	{
		Vertex = Vertex.RotateAngleAxis(Rotation.Yaw, FVector::UpVector);
		Vertex += StartLocation;
	}

	Data.Triangles = TArray<int32>({
		0, 2, 1,
		2, 3, 1
	});

	const auto UV = Length / Height;
	Data.UVs = TArray<FVector2D>({
		{0, 0},
		{UV, 0},
		{0, 1},
		{UV, 1},
	});

	const auto CachedSide = Data.Triangles;
	Algo::Reverse(Data.Triangles);
	Data.Triangles.Append(CachedSide);

	CreateMeshSection(
		0,
		Data.Vertices,
		Data.Triangles,
		Data.Normals,
		Data.UVs,
		TArray<FColor>(),
		Data.Tangents,
		true
	);
}

void UGLibProceduralMesh::CreatePlaneWallOnGrid(
	const FVector& StartLocation,
	const FVector& EndLocation,
	const float Height,
	const float GridSize,
	FGeneratedWallData& Data
)
{
	const auto GridStartLocation = UKismetMathLibrary::Vector_SnappedToGrid(StartLocation, GridSize);
	const auto GridEndLocation = UKismetMathLibrary::Vector_SnappedToGrid(EndLocation, GridSize);

	CreatePlaneWall(GridStartLocation, GridEndLocation, Height, Data);
}
