// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Controllers/GLibPlayerController.h"
#include "GLibAbilityTask_WaitForKeyInput.generated.h"

/**
 * @brief Ability task that waits key input
 */
UCLASS()
class GRADESSGASLIBRARY_API UGLibAbilityTask_WaitForKeyInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * @brief Fires when key was pressed
	 */
	UPROPERTY(
		BlueprintAssignable
	)
	FGLibOnKeyStateChanged Pressed;

	/**
	 * @brief Fires when key was released
	 */
	UPROPERTY(
		BlueprintAssignable
	)
	FGLibOnKeyStateChanged Released;

	/**
	 * @brief Creates task for key listening 
	 * @param OwningAbility current ability
	 * @param Keys keys to listen
	 * @param bInTriggerAlreadyPressed fires delegates after activation for currently pressed keys
	 * @return created task
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "Ability|Tasks",
		meta = (
			HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "True",
			AutoCreateRefTerm = "Keys"
		)
	)
	static UGLibAbilityTask_WaitForKeyInput* WaitForKeyInput(
		UGameplayAbility* OwningAbility,
		const TArray<FKey>& Keys,
		UPARAM(DisplayName = "TriggerAlreadyPressed") const bool bInTriggerAlreadyPressed
	);

	/**
	 * @brief Fires when key was pressed
	 * @param Key pressed ley
	 */
	UFUNCTION()
	void OnPressed(FKey Key);
	
	/**
	 * @brief Fires when key was released
	 * @param Key released key
	 */
	UFUNCTION()
	void OnReleased(FKey Key);

	virtual void Activate() override;

protected:
	/**
	 * @brief Keys to listen and send in Pressed and Released events
	 */
	UPROPERTY()
	TMap<FName, FKey> ListenKeys;

	UPROPERTY()
	bool bTriggerAlreadyPressed;
};
