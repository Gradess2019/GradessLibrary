// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibPlayerController.generated.h"

#pragma region Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnKeyStateChanged, FKey, Key);
#pragma endregion Delegate declarations

/**
 * @brief Custom player controller that has OnKeyPressed delegate
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	)
)
class GRADESSLIBRARYRUNTIME_API AGLibPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * @brief Fires when player press any key
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|PlayerController"
	)
	FGLibOnKeyStateChanged OnKeyPressed;

	/**
	 * @brief Fires when player releases any key
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|PlayerController"
	)
	FGLibOnKeyStateChanged OnKeyReleased;
	
	virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
};
