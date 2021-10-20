// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GLibEditorUserInterfaceLibrary.generated.h"

#pragma region Forward declaractions
class UEditorUtilitySubsystem;
class UEditorUtilityWidgetBlueprint;
class UEditorUtilityWidget;
#pragma endregion Forward declaractions


/**
 * @brief A library that is used for working with editor user widgets
 */
UCLASS()
class GRADESSLIBRARYEDITOR_API UGLibEditorUserInterfaceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UGLibEditorUserInterfaceLibrary();

	/**
	* @brief Enables auto closing for widget when editor is closed
	* @param Widget widget to close when editor is closing. Default is self
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static void EnableAutoClosing(UEditorUtilityWidget* Widget);

	/**
	* @brief Registers editor widget and save it to RegisteredWidgets.ini
	* It gives us a possibility to not reload editor widget every startup
	* @param Widget widget to open after engine restart. Default is self
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static void RegisterEditorWidget(UEditorUtilityWidget* Widget);

	/**
	* @brief Unregisters editor widget and removes it from RegisteredWidgets.ini after editor closing
	* @param Widget widget to unregister. Default is self
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static void UnregisterEditorWidget(UEditorUtilityWidget* Widget);

	/**
	* @brief Saves registered widgets into RegisteredWidgets.ini
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void SaveRegisteredWidgets();

	/**
	* @brief Opens registered widgets
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void OpenWidgets();

	/**
	* @brief Rebinds SaveAndClear() function to OnEditorClosed to call this function first
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void RebindSaveDelegate();

	/**
	* @brief Saves registered widgets to config file and reset RegisteredWidgets set
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void SaveAndClear();

	/**
	* @brief Loads widgets array from config into RegisteredWidgets set
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void LoadWidgetsFromConfig();

	/**
	* @brief Reloads widget asset from disk
	* @param WidgetPath widget to reload
	* @param EditorSubsystem editor system to use
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static void ReloadWidget(
		const FString& WidgetPath,
		UEditorUtilitySubsystem* EditorSubsystem
	);

	/**
	* @brief Spawns editor widget by path
	* @param WidgetPath widget to spawn
	* @param EditorSubsystem editor system to use
	*/
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static bool SpawnWidget(
		const FString& WidgetPath,
		UEditorUtilitySubsystem* EditorSubsystem
	);

	/**
	* @brief Gets widget tab id
	* @param Widget widget to get it's tab id. Default to self
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static FName GetWidgetTabId(const UEditorUtilityWidget* Widget);

	/**
	* @brief Returns widget tab id by asset name
	* @param AssetName widget asset name with path
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static FName GetWidgetTabIdByAssetName(FString AssetName);

	/**
	* @brief Returns widget blueprint from widget instance
	* @param Widget widget to obtain blueprint. Default to self
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static UEditorUtilityWidgetBlueprint* GetWidgetBlueprint(const UEditorUtilityWidget* Widget);

	/**
	* @brief Returns widget blueprint path from widget instance
	* @param Widget widget to obtain blueprint path. Default to self
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|EditorUserInterfaceLibrary",
		meta = (
			DefaultToSelf="Widget"
		)
	)
	static FString GetWidgetBlueprintPath(const UEditorUtilityWidget* Widget);

	/**
	* @brief Returns config file path
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|EditorUserInterfaceLibrary"
	)
	static FString GetConfigPath();

private:
	/**
	* @brief Returns name of the config section
	*/
	UFUNCTION()
	static FString GetSection();

	/**
	* @brief Returns name of the config key
	*/
	UFUNCTION()
	static FString GetKey();

private:
	static TSet<FString> RegisteredWidgets;
	static TMap<FString, FDelegateHandle> BoundWidgets;
	static FDelegateHandle SaveConfigDelegate;
};
