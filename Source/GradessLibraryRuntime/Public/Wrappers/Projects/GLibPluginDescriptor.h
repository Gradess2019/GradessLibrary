// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GLibCustomBuildSteps.h"
#include "GLibLocalizationDescriptor.h"
#include "GLibPluginReferenceDescriptor.h"
#include "GLibPluginDescriptor.generated.h"


struct FPluginDescriptor;
struct FGLibModuleDescriptor;


/**
* Setting for whether a plugin is enabled by default
*/
UENUM(BlueprintType)
enum class EGLibPluginEnabledByDefault : uint8
{
	Unspecified = 0,
	Enabled = 1,
	Disabled = 2
};


/**
* Descriptor for plugins. Contains all the information contained within a .uplugin file.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibPluginDescriptor
{
	GENERATED_BODY()

public:
	FGLibPluginDescriptor();
	FGLibPluginDescriptor(const FPluginDescriptor& InDescriptor);
	
	operator FPluginDescriptor() const;
	
	/**
	 * Version number for the plugin.  The version number must increase with every version of the plugin, so that the system
     * can determine whether one version of a plugin is newer than another, or to enforce other requirements.  This version
     * number is not displayed in front-facing UI.  Use the VersionName for that.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	int32 Version;

	/**
	 * Name of the version for this plugin.  This is the front-facing part of the version number.  It doesn't need to match
	 * the version number numerically, but should be updated when the version number is increased accordingly.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString VersionName;

	/** Friendly name of the plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString FriendlyName;

	/** Description of the plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString Description;

	/** The name of the category this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString Category;

	/** The company or individual who created this plugin.  This is an optional field that may be displayed in the user interface. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString CreatedBy;

	/** Hyperlink URL string for the company or individual who created this plugin.  This is optional. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString CreatedByURL;

	/** Documentation URL string. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString DocsURL;

	/** Marketplace URL for this plugin. This URL will be embedded into projects that enable this plugin, so we can redirect to the marketplace if a user doesn't have it installed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString MarketplaceURL;

	/** Support URL/email for this plugin. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString SupportURL;

	/** Version of the engine that this plugin is compatible with */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString EngineVersion;

	/** Optional custom virtual path to display in editor to better organize. Inserted just before this plugin's directory in the path: /All/Plugins/EditorCustomVirtualPath/PluginName */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString EditorCustomVirtualPath;

	/** Controls a subset of platforms that can use this plugin, and which ones will stage the .uplugin file and content files.
		Generally, for code plugins, it should be the union of platforms that the modules in the plugin are compiled for. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	TArray<FString> SupportedTargetPlatforms;

	/** List of programs that are supported by this plugin. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	TArray<FString> SupportedPrograms;

	/** If specified, this is the real plugin that this one is just extending */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FString ParentPluginName;

	/** List of all modules associated with this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	TArray<FGLibModuleDescriptor> Modules;

	/** List of all localization targets associated with this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	TArray<FGLibLocalizationTargetDescriptor> LocalizationTargets;

	/** Whether this plugin should be enabled by default for all projects */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	EGLibPluginEnabledByDefault EnabledByDefault;

	/** Can this plugin contain content? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bCanContainContent;

	/** Can this plugin contain Verse code? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bCanContainVerse;

	/** Marks the plugin as beta in the UI */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bIsBetaVersion;

	/** Marks the plugin as experimental in the UI */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bIsExperimentalVersion;

	/** Signifies that the plugin was installed on top of the engine */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bInstalled;

	/** For plugins that are under a platform folder (eg. /PS4/), determines whether compiling the plugin requires the build platform and/or SDK to be available */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bRequiresBuildPlatform;

	/** For auto-generated plugins that should not be listed in the plugin browser for users to disable freely. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bIsHidden;

	/** When true, this plugin's modules will not be loaded automatically nor will it's content be mounted automatically. It will load/mount when explicitly requested and LoadingPhases will be ignored */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bExplicitlyLoaded;

	/** When true, an empty SupportedTargetPlatforms is interpreted as 'no platforms' with the expectation that explicit platforms will be added in plugin platform extensions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bHasExplicitPlatforms;

	/** If true, this plugin from a platform extension extending another plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	bool bIsPluginExtension;

	/** Pre-build steps for each host platform */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FGLibCustomBuildSteps PreBuildSteps;

	/** Post-build steps for each host platform */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	FGLibCustomBuildSteps PostBuildSteps;

	/** Plugins used by this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	TArray<FGLibPluginReferenceDescriptor> Plugins;

	// TODO
	// /** Additional fields to write */
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginDescriptor")
	// TMap<FString,TSharedPtr<FJsonValue>> AdditionalFieldsToWrite;

public:
	/** Return the .uplugin extension (with dot) */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// static const FString& GetFileExtension();

	/** Constructor. */
	// FGLibPluginDescriptor();

	/** Loads the descriptor from the given file. */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Load(const FString& FileName, FText * OutFailReason = nullptr);

	/** Loads the descriptor from the given file. */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Load2(const FString& FileName, FText& OutFailReason);

	/** Reads the descriptor from the given string */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Read(const FString& Text, FText * OutFailReason = nullptr);

	/** Reads the descriptor from the given string */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Read2(const FString& Text, FText& OutFailReason);

	/** Reads the descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Read2(const FJsonObject& Object, FText * OutFailReason = nullptr);

	/** Reads the descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// bool Read2(const FJsonObject& Object, FText& OutFailReason);

	/** Saves the descriptor from the given file. */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginDescriptor")
	// bool Save(const FString& FileName, FText * OutFailReason = nullptr);

	/** Saves the descriptor from the given file. */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginDescriptor")
	// bool Save2(const FString& FileName, FText& OutFailReason);

	/** Writes a descriptor to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// void Write(FString& Text);

	/** Writes a descriptor to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// void Write2(TJsonWriter<>& Writer);

	/** Updates the given json object with values in this descriptor */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginDescriptor")
	// void UpdateJson(FJsonObject& JsonObject);

	/**
	* Updates the content of the specified plugin file with values in this descriptor
	* (hence preserving json fields that the plugin descriptor doesn't know about)
	*/
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginDescriptor")
	// bool UpdatePluginFile(const FString& FileName, FText * OutFailReason = nullptr);

	/**
	* Updates the content of the specified plugin file with values in this descriptor
	* (hence preserving json fields that the plugin descriptor doesn't know about)
	*/
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginDescriptor")
	// bool UpdatePluginFile2(const FString& FileName, FText& OutFailReason);

	/** Determines whether the plugin supports the given platform */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginDescriptor")
	// bool SupportsTargetPlatform(const FString& Platform);

};

