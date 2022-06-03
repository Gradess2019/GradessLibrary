// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomBuildSteps.h"
#include "GLibCustomBuildSteps.generated.h"


USTRUCT(BlueprintType)
struct FGLibHostPlatformCommands
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|CustomBuildSteps")
	TArray<FString> Commands;

	FGLibHostPlatformCommands(const TArray<FString>& InCommands = TArray<FString>()) { Commands = InCommands; }
	operator TArray<FString>() const { return Commands; }
};

/**
* Descriptor for projects. Contains all the information contained within a .uproject file.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibCustomBuildSteps
{
	GENERATED_BODY()
	
public:
	FGLibCustomBuildSteps();
	FGLibCustomBuildSteps(const FCustomBuildSteps& Native);

	operator FCustomBuildSteps() const;

	/** Mapping from host platform to list of commands */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|CustomBuildSteps")
	TMap<FString, FGLibHostPlatformCommands> HostPlatformToCommands;

public:
	// TODO
	// /** Whether this object is empty */
	// UFUNCTION(BlueprintPure, Category = "GLib|CustomBuildSteps")
	// bool IsEmpty();
	//
	// /** Reads the descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|CustomBuildSteps")
	// void Read(const FJsonObject& Object, const FString& FieldName);
	//
	// /** Writes the CustomBuildSteps to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|CustomBuildSteps")
	// void Write(TJsonWriter<>& Writer, const FString& FieldName);
	//
	// /** Updates the given json object with this CustomBuildSteps */
	// UFUNCTION(BlueprintCallable, Category = "GLib|CustomBuildSteps")
	// void UpdateJson(FJsonObject& JsonObject, const FString& FieldName);
};
