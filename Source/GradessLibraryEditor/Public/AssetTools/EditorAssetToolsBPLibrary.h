// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorAssetToolsBPLibrary.generated.h"

/**
 * @brief A function library that provides additional useful functions such as unloading assets in editor
 */
UCLASS()
class GRADESSLIBRARYEDITOR_API UEditorAssetToolsBPLibrary : public UBlueprintFunctionLibrary
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
		Category = "GLib|EditorAssetToolsBPLibrary"
	)
	static UPackage* GetPackage(const FName& PackagePath);

	/**
	 * @brief Unloads package by path
	 * @param PackagePath package to unload
	 * @return true if successful, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsBPLibrary"
	)
	static bool UnloadPackage(const FName& PackagePath);

	/**
	 * @brief Unloads a package and its descendants, excluding assets that is currently referenced by other loaded assets
	 * @param PackagePath package to unload
 	 * @return true if successful, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsBPLibrary"
	)
	static bool UnloadPackageAndDependencies(const FName& PackagePath);

	/**
	 * @brief Unloads dependencies
	 * @param Dependencies dependencies to unload
	 * @param OutChildDependencies dependencies of passed dependencies
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorAssetToolsBPLibrary"
	)
	static bool UnloadDependencies(
		UPARAM(ref) TSet<FName>& Dependencies,
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
		Category = "GLib|EditorAssetToolsBPLibrary"
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
		Category = "GLib|EditorAssetToolsBPLibrary"
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
		Category = "GLib|EditorAssetToolsBPLibrary"
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
		Category = "GLib|EditorAssetToolsBPLibrary"
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
		Category = "GLib|EditorAssetToolsBPLibrary"
	)
	static bool IsAnybodyReferenced(const FName& PackagePath);
};
