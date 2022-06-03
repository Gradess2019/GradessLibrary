// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PluginReferenceDescriptor.h"

#include "UObject/Object.h"

#include "Wrappers/Core/GLibGenericPlatformMisc.h"

#include "GLibPluginReferenceDescriptor.generated.h"

/**
* Descriptor for a plugin reference. Contains the information required to enable or disable a plugin for a given platform.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibPluginReferenceDescriptor
{
	GENERATED_BODY()

public:
	/** Name of the plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	FString Name;

	/** Whether it should be enabled by default */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	bool bEnabled;

	/** Whether this plugin is optional, and the game should silently ignore it not being present */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	bool bOptional;

	/** Description of the plugin for users that do not have it installed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	FString Description;

	/** URL for this plugin on the marketplace, if the user doesn't have it installed. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	FString MarketplaceURL;

	/** If enabled, list of platforms for which the plugin should be enabled (or all platforms if blank). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<FString> PlatformAllowList;

	/** If enabled, list of platforms for which the plugin should be disabled. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<FString> PlatformDenyList;

	/** If enabled, list of target configurations for which the plugin should be enabled (or all target configurations if blank). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<EGLibBuildConfiguration> TargetConfigurationAllowList;

	/** If enabled, list of target configurations for which the plugin should be disabled. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<EGLibBuildConfiguration> TargetConfigurationDenyList;

	/** If enabled, list of targets for which the plugin should be enabled (or all targets if blank). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<EGLibBuildTargetType> TargetAllowList;

	/** If enabled, list of targets for which the plugin should be disabled. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<EGLibBuildTargetType> TargetDenyList;

	/** The list of supported target platforms for this plugin. This field is copied from the plugin descriptor, and supplements the user's allowed/denied platforms. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	TArray<FString> SupportedTargetPlatforms;

	/** When true, empty SupportedTargetPlatforms and PlatformAllowList are interpreted as 'no platforms' with the expectation that explicit platforms will be added in plugin platform extensions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|PluginReferenceDescriptor")
	bool bHasExplicitPlatforms;

public:
	/** Constructor */
	FGLibPluginReferenceDescriptor(const FString& InName = TEXT(""), bool bInEnabled = false);
	FGLibPluginReferenceDescriptor(const FPluginReferenceDescriptor& InDescriptor);

	operator FPluginReferenceDescriptor() const;

	// TODO
	// /** Determines whether the plugin is enabled for the given platform */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginReferenceDescriptor")
	// bool IsEnabledForPlatform(const FString& Platform);
	//
	// /** Determines whether the plugin is enabled for the given target configuration */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginReferenceDescriptor")
	// bool IsEnabledForTargetConfiguration(EBuildConfiguration Configuration);
	//
	// /** Determines whether the plugin is enabled for the given target */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginReferenceDescriptor")
	// bool IsEnabledForTarget(EBuildTargetType TargetType);
	//
	// /** Determines if the referenced plugin is supported for the given platform */
	// UFUNCTION(BlueprintPure, Category = "GLib|PluginReferenceDescriptor")
	// bool IsSupportedTargetPlatform(const FString& Platform);
	//
	// /** Reads the descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// bool Read(const FJsonObject& Object, FText * OutFailReason = nullptr);
	//
	// /** Reads the descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// bool Read2(const FJsonObject& Object, FText& OutFailReason);
	//
	// /** Reads an array of modules from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// static bool ReadArray(const FJsonObject& Object, const TCHAR * Name, TArray<FGLibPluginReferenceDescriptor>& OutModules, FText * OutFailReason = nullptr);
	//
	// /** Reads an array of modules from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// static bool ReadArray2(const FJsonObject& Object, const TCHAR * Name, TArray<FGLibPluginReferenceDescriptor>& OutModules, FText& OutFailReason);
	//
	// /** Writes a descriptor to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// void Write(TJsonWriter<>& Writer);
	//
	// /** Updates the given json object with values in this descriptor */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// void UpdateJson(FJsonObject& JsonObject);
	//
	// /** Writes an array of plugin references to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// static void WriteArray(TJsonWriter<>& Writer, const TCHAR * ArrayName, const TArray<FGLibPluginReferenceDescriptor>& Plugins);
	//
	// /** Updates an array of plugin references in the specified JSON field (indexed by plugin name) */
	// UFUNCTION(BlueprintCallable, Category = "GLib|PluginReferenceDescriptor")
	// static void UpdateArray(FJsonObject& JsonObject, const TCHAR * ArrayName, const TArray<FGLibPluginReferenceDescriptor>& Plugins);

};