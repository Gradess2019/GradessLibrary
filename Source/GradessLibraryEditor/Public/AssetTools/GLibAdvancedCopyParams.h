// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "GLibAdvancedCopyParams.generated.h"

/**
 * @brief A proxy blueprint struct that is used for advanced copy 
 */
USTRUCT(BlueprintType)
struct FGLibAdvancedCopyParams
{
	GENERATED_BODY()

	FGLibAdvancedCopyParams(
		const bool ShouldForceSave = true,
		const bool CopyOverAllDestinationOverlaps = true,
		const bool GenerateUniqueNames = false,
		const bool ShouldSuppressUI = true,
		const bool ShouldCheckForDependencies = false
	) :
		bShouldForceSave(ShouldForceSave),
		bCopyOverAllDestinationOverlaps(CopyOverAllDestinationOverlaps),
		bGenerateUniqueNames(GenerateUniqueNames),
		bShouldSuppressUI(ShouldSuppressUI),
		bShouldCheckForDependencies(ShouldCheckForDependencies) {}

	FGLibAdvancedCopyParams(
		const FAdvancedCopyParams& Params
	) :
		bShouldForceSave(Params.bShouldForceSave),
		bCopyOverAllDestinationOverlaps(Params.bCopyOverAllDestinationOverlaps),
		bGenerateUniqueNames(Params.bGenerateUniqueNames),
		bShouldSuppressUI(Params.bShouldSuppressUI),
		bShouldCheckForDependencies(Params.bShouldCheckForDependencies) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GLib")
	bool bShouldForceSave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GLib")
	bool bCopyOverAllDestinationOverlaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GLib")
	bool bGenerateUniqueNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GLib")
	bool bShouldSuppressUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GLib")
	bool bShouldCheckForDependencies;

	FORCEINLINE FAdvancedCopyParams ToNative() const
	{
		FAdvancedCopyParams Params;
		CopyToNative(Params);
		
		return Params;
	}

	FORCEINLINE void CopyToNative(FAdvancedCopyParams& Params) const
	{
		Params.bShouldForceSave = bShouldForceSave;
		Params.bCopyOverAllDestinationOverlaps = bCopyOverAllDestinationOverlaps;
		Params.bGenerateUniqueNames = bGenerateUniqueNames;
		Params.bShouldSuppressUI = bShouldSuppressUI;
		Params.bShouldCheckForDependencies = bShouldCheckForDependencies;
	}
};