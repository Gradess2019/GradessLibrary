// Copyright Gradess. All Rights Reserved.


#include "Core/GLibMathLibrary.h"

float UGLibMathLibrary::ShortestAngleBetweenVectorsInRadians(
	const FVector& FirstVector,
	const FVector& SecondVector
)
{
	const auto BetweenQuat = FQuat::FindBetween(FirstVector, SecondVector);

	auto Axis = FVector();
	auto Angle = 0.f;
	BetweenQuat.ToAxisAndAngle(Axis, Angle);

	return Angle;
}

float UGLibMathLibrary::ShortestAngleBetweenVectorsInDegrees(const FVector& FirstVector, const FVector& SecondVector)
{
	const auto Radians = ShortestAngleBetweenVectorsInRadians(FirstVector, SecondVector);
	return FMath::RadiansToDegrees(Radians);
}
