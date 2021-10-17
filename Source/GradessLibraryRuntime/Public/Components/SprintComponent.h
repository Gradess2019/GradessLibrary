﻿// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SprintComponent.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USprintComponent : public UInterface
{
	GENERATED_BODY()
};


/**
 * @brief Represent an object that is responsible for player sprint movement
 */
class GRADESSLIBRARYRUNTIME_API ISprintComponent
{
	GENERATED_BODY()

public:

	/**
	 * @brief Enable sprinting mode for owner
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|MovementComponent"
	)
	void Sprint();

	/**
	 * @brief Disable sprinting mode for owner
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintNativeEvent,
		Category = "GLib|MovementComponent"
	)
	void UnSprint();
};
