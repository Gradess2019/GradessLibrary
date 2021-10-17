// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
class GRADESSLIBRARYRUNTIME_API UGLibFirstPersonMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UGLibFirstPersonMovementComponent();

	virtual float GetMaxSpeed() const override;

	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|MovementComponent"
	)
	void Sprint();

	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|MovementComponent"
	)
	void UnSprint();

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
