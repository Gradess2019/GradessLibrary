// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GLibSimpleInteractable.generated.h"

UINTERFACE()
class UGLibSimpleInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief A simple interface for interactable objects
 */
class GRADESSLIBRARYRUNTIME_API IGLibSimpleInteractable
{
	GENERATED_BODY()

public:
	/**
	 * @brief Interacts with current actor
	 * @param Instigator causer of event
	 * @return true if interacted successfully, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|SimpleInteractable"
	)
	bool Interact(AActor* Instigator);

};
