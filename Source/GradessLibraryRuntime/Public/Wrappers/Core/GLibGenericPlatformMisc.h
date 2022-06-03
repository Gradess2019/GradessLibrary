// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GLibGenericPlatformMisc.generated.h"

/**
* Enumerates build target types.
*/
UENUM(BlueprintType)
enum class EGLibBuildTargetType : uint8
{
	/** Unknown build target. */
	Unknown = 0,
	/** Game target. */
	Game = 1,
	/** Server target. */
	Server = 2,
	/** Client target. */
	Client = 3,
	/** Editor target. */
	Editor = 4,
	/** Program target. */
	Program = 5
};

/**
* Available build configurations. Mirorred from UnrealTargetConfiguration.
*/
UENUM(BlueprintType)
enum class EGLibBuildConfiguration : uint8
{
	/** Unknown build configuration. */
	Unknown = 0,
	/** Debug build. */
	Debug = 1,
	/** DebugGame build. */
	DebugGame = 2,
	/** Development build. */
	Development = 3,
	/** Shipping build. */
	Shipping = 4,
	/** Test build. */
	Test = 5
};
