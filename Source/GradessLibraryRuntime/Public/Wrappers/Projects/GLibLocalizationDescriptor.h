// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GLibLocalizationDescriptor.generated.h"

struct FLocalizationTargetDescriptor;
/**
* Policy by which the localization data associated with a target should be loaded.
*/
UENUM(BlueprintType)
enum class EGLibLocalizationTargetDescriptorLoadingPolicy : uint8
{
	/** The localization data will never be loaded automatically. */
	Never = 0,
	/** The localization data will always be loaded automatically. */
	Always = 1,
	/** The localization data will only be loaded when running the editor. Use if the target localizes the editor. */
	Editor = 2,
	/** The localization data will only be loaded when running the game. Use if the target localizes your game. */
	Game = 3,
	/** The localization data will only be loaded if the editor is displaying localized property names. */
	PropertyNames = 4,
	/** The localization data will only be loaded if the editor is displaying localized tool tips. */
	ToolTips = 5,
	/** NOTE: If you add a new value, make sure to update the ToString() method below!. */
	Max = 6,
};

/**
* Description of a localization target.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibLocalizationTargetDescriptor
{
	GENERATED_BODY()

public:
	/** Name of this target */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LocalizationTargetDescriptor")
	FString Name;

	/** When should the localization data associated with a target should be loaded? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LocalizationTargetDescriptor")
	EGLibLocalizationTargetDescriptorLoadingPolicy LoadingPolicy;

public:
	/** Normal constructor */
	FGLibLocalizationTargetDescriptor(FString InName = FString(), EGLibLocalizationTargetDescriptorLoadingPolicy InLoadingPolicy = EGLibLocalizationTargetDescriptorLoadingPolicy::Never);
	FGLibLocalizationTargetDescriptor(const FLocalizationTargetDescriptor& InDescriptor);
	
	operator FLocalizationTargetDescriptor() const;

	// TODO: GLIB-14
	// /** Reads a descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// bool Read(const FJsonObject& InObject, FText * OutFailReason = nullptr);
	//
	// /** Reads a descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// bool Read2(const FJsonObject& InObject, FText& OutFailReason);
	//
	// /** Reads an array of targets from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// static bool ReadArray(const FJsonObject& InObject, const TCHAR * InName, TArray<FGLibLocalizationTargetDescriptor>& OutTargets, FText * OutFailReason = nullptr);
	//
	// /** Reads an array of targets from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// static bool ReadArray2(const FJsonObject& InObject, const TCHAR * InName, TArray<FGLibLocalizationTargetDescriptor>& OutTargets, FText& OutFailReason);
	//
	// /** Writes a descriptor to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// void Write(TJsonWriter<>& Writer);
	//
	// /** Updates the given json object with values in this descriptor */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// void UpdateJson(FJsonObject& JsonObject);
	//
	// /** Writes an array of targets to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// static void WriteArray(TJsonWriter<>& Writer, const TCHAR * ArrayName, const TArray<FGLibLocalizationTargetDescriptor>& Descriptors);
	//
	// /** Updates an array of descriptors in the specified JSON field (indexed by name) */
	// UFUNCTION(BlueprintCallable, Category = "GLib|LocalizationTargetDescriptor")
	// static void UpdateArray(FJsonObject& JsonObject, const TCHAR * ArrayName, const TArray<FGLibLocalizationTargetDescriptor>& Descriptors);
	//
	// /** Returns true if we should load this localization target based upon the current runtime environment */
	// UFUNCTION(BlueprintPure, Category = "GLib|LocalizationTargetDescriptor")
	// bool ShouldLoadLocalizationTarget();

};


