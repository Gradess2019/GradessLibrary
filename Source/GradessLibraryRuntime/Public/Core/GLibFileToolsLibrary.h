// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibFileToolsLibrary.generated.h"

/**
 * @brief A library that is used for working with files on machine
 */
UCLASS()
class GRADESSLIBRARYRUNTIME_API UGLibFileToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Creates config file in project config directory if it doesnt exist
	 * @param FileName config file name
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|FileToolsLibrary"
	)
	static void ValidateConfigFile(
		const FString& FileName
	);

	/**
	 * @brief Creates file by path
	 * @param FilePath config file name
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|FileToolsLibrary"
	)
	static void CreateFile(
		const FString& FilePath
	);
};
