// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibContentBrowserExtras.generated.h"

/**
 * @brief Asset view type in content browser
 */
UENUM(
	BlueprintType
)
enum EGLibAssetViewType
{
	List,
	Tile,
	Column,

	MAX
};