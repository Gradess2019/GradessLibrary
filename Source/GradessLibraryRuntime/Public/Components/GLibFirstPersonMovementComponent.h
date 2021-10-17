﻿// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SprintComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GLibFirstPersonMovementComponent.generated.h"


/**
 * @brief Custom movement component that implements sprinting
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	),
	meta = (
		BlueprintSpawnableComponent
	)
)
class GRADESSLIBRARYRUNTIME_API UGLibFirstPersonMovementComponent : public UCharacterMovementComponent, public ISprintComponent
{
	GENERATED_BODY()

public:
	UGLibFirstPersonMovementComponent();

	virtual float GetMaxSpeed() const override;

#pragma region ISprintComponent implementation
	virtual void Sprint_Implementation() override;
	virtual void UnSprint_Implementation() override;
#pragma endregion ISprintComponent implementation


	UPROPERTY(
		BlueprintReadWrite,
		Category = "GLib|MovementComponent"
	)
	float MaxWalkSpeedSprinted;

	UPROPERTY(
		BlueprintReadWrite,
		Category = "GLib|MovementComponent"
	)
	bool bSprint;
};
