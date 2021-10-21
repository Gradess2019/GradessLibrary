// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibSprintComponent.h"
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
class GRADESSLIBRARYRUNTIME_API UGLibFirstPersonMovementComponent : public UCharacterMovementComponent, public IGLibSprintComponent
{
	GENERATED_BODY()

public:
	UGLibFirstPersonMovementComponent();

	virtual float GetMaxSpeed() const override;

#pragma region ISprintComponent implementation
	virtual void Sprint_Implementation() override;
	virtual void UnSprint_Implementation() override;
#pragma endregion ISprintComponent implementation


	/**
	 * @brief Max speed character can reach while sprinting
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|FirstPersonMovementComponent"
	)
	float MaxWalkSpeedSprinted;

	/**
	 * @brief Whether sprinting or not
	 */
	UPROPERTY(
		BlueprintReadWrite,
		Category = "GLib|FirstPersonMovementComponent"
	)
	bool bSprint;
};
