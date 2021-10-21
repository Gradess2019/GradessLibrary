// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibAssetThumbnail.generated.h"

/**
 * @brief Thumbnail label type
 */
UENUM(
	BlueprintType
)
enum EGLibThumbnailLabel
{
	ClassName,
	AssetName,
	NoLabel
};
