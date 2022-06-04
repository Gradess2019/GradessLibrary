// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GLibModuleManager.generated.h"

/**
* Enumerates reasons for failed module loads.
*/
UENUM(BlueprintType)
enum class EGLibModuleLoadResult : uint8
{
	/** Module loaded successfully. */
	Success = 0,
	/** The specified module file could not be found. */
	FileNotFound = 1,
	/** The specified module file is incompatible with the module system. */
	FileIncompatible = 2,
	/** The operating system failed to load the module file. */
	CouldNotBeLoadedByOS = 3,
	/** Module initialization failed. */
	FailedToInitialize = 4
};

/**
* Enum for where a plugin is loaded from
*/
UENUM(BlueprintType)
enum class EGLibPluginLoadedFrom : uint8
{
	/** Plugin is built-in to the engine */
	Engine = 0,
	/** Project-specific plugin, stored within a game project directory */
	Project = 1
};