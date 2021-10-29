// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibMathLibrary.generated.h"


/**
 * @brief Library that contains additional auxiliary math operations and function
 */
UCLASS()
class GRADESSLIBRARYRUNTIME_API UGLibMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns shortest angle between vectors in radians
	 * @param FirstVector vector to use
	 * @param SecondVector vector to use
	 * @return shortest angle in radians
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|MathLibrary",
		meta = (
			CompactNodeTitle = "AngleRad"
		)
	)
	static float ShortestAngleBetweenVectorsInRadians(
		const FVector& FirstVector,
		const FVector& SecondVector
	);

	/**
	 * @brief Returns shortest angle between vectors in radians
	 * @param FirstVector vector to use
	 * @param SecondVector vector to use
	 * @return shortest angle in radians
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|MathLibrary",
		meta = (
			CompactNodeTitle = "AngleDeg"
		)
	)
	static float ShortestAngleBetweenVectorsInDegrees(
		const FVector& FirstVector,
		const FVector& SecondVector
	);
};
