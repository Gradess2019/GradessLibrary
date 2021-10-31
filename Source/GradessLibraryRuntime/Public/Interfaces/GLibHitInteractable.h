// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GLibHitInteractable.generated.h"

UINTERFACE()
class UGLibHitInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief An interact interface with hit
 */
class GRADESSLIBRARYRUNTIME_API IGLibHitInteractable
{
	GENERATED_BODY()

public:
	/**
	 * @brief Interacts with current actor
	 * @param Instigator causer of event
	 * @param HitResult hit that is made by causer
	 * @return true if interacted successfully, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|HitInteractable"
	)
	bool Interact(
		AActor* Instigator,
		const FHitResult& HitResult
	);
};
