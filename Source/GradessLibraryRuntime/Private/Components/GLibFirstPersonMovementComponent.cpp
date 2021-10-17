// Copyright Gradess. All Rights Reserved.


#include "Components/GLibFirstPersonMovementComponent.h"

UGLibFirstPersonMovementComponent::UGLibFirstPersonMovementComponent()
{
	MaxWalkSpeedSprinted = 1.5f * MaxWalkSpeed;
}

float UGLibFirstPersonMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		// TODO: Improve flexibility through State pattern
		return IsCrouching() ? MaxWalkSpeedCrouched : bSprint ? MaxWalkSpeedSprinted : MaxWalkSpeed;
	default:
		return Super::GetMaxSpeed();
	}
}

void UGLibFirstPersonMovementComponent::Sprint_Implementation()
{
	if (IsCrouching()) { return; }
	bSprint = true;
}

void UGLibFirstPersonMovementComponent::UnSprint_Implementation()
{
	bSprint = false;
}
