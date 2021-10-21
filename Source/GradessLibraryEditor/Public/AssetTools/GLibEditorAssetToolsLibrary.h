// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibAdvancedCopyParams.h"
#include "GLibEditorAssetToolsLibrary.generated.h"

/**
 * @brief A function library that provides additional useful functions such as unloading assets in editor
 */
UCLASS()
class GRADESSLIBRARYEDITOR_API UGLibEditorAssetToolsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns UPackage object by path
	 * @param PackagePath path to package
	 * @return package object
	 */
	UFUNCTION(
		BlueprintPure,
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static UPackage* GetPackage(const FName& PackagePath);

	/**
	 * @brief Unloads package by path
	 * @param PackagePath package to unload
	 * @return true if successful, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static bool UnloadPackage(const FName& PackagePath);

	/**
	 * @brief Unloads a package and its descendants, excluding assets that is currently referenced by other loaded assets
	 * @param PackagePath package to unload
 	 * @return true if successful, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static bool UnloadPackageAndDependencies(const FName& PackagePath);

	/**
	 * @brief Unloads dependencies
	 * @param Dependencies dependencies to unload
	 * @param OutChildDependencies dependencies of passed dependencies
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static bool UnloadDependencies(
		const TSet<FName>& Dependencies,
		UPARAM(DisplayName="ChildDependencies") TSet<FName>& OutChildDependencies
	);

	/**
	 * @brief Returns set of unreferenced packages
	 * @param PackagePaths array of packages to check
	 * @param OutUnreferencedPackagePaths set of unreferenced, but loaded packages
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void GetUnreferencedPackages(
		const TArray<FName>& PackagePaths,
		UPARAM(DisplayName="UnreferencedPackages") TSet<FName>& OutUnreferencedPackagePaths
	);

	/**
	 * @brief Returns package hard dependencies 
	 * @param PackagePath package to obtain hard dependencies
	 * @param OutDependencies obtained dependencies
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void GetHardDependencies(
		const FName& PackagePath,
		UPARAM(DisplayName="Dependencies") TArray<FName>& OutDependencies
	);

	/**
	* @brief Returns package hard referencers 
	* @param PackagePath package to obtain hard referencers
	* @param OutReferencers obtained referencers
	*/
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void GetHardReferencers(
		const FName& PackagePath,
		UPARAM(DisplayName="Referencers") TArray<FName>& OutReferencers
	);

	/**
	 * @brief Checks if given package is a project package
	 * @param Package package to check
	 * @return true if package is in "/Game" directory, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static bool IsProjectPackage(const FName& Package);

	/**
	 * @brief Checks if anybody refers to the given package
	 * @param PackagePath package to check
	 * @return true if someone refers, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static bool IsAnybodyReferenced(const FName& PackagePath);

	/**
	* @brief Copies directory from source to destination with reference fixing
	* @param Source directory that will be copied
	* @param Destination directory where copied source will be placed
	* @param Params advanced copy params
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary",
		meta = (AutoCreateRefTerm = "Params")
	)
	static void AdvancedCopyDirectory(
		const FString Source,
		const FString Destination,
		const FGLibAdvancedCopyParams& Params
	);

	/**
	* @brief Reloads asset provided by asset data
	* @param AssetData asset to reload
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void ReloadAsset(
		const FAssetData& AssetData
	);

	/**
	* @brief Reloads asset by exact path
	* @param AssetPath path to asset to reload
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void ReloadAssetByPath(
		const FName& AssetPath
	);

	/**
	* @brief Reloads bunch of assets by asset data
	* @param AssetsData assets to reload
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsLibrary"
	)
	static void ReloadAssets(
		const TArray<FAssetData>& AssetsData
	);
};
