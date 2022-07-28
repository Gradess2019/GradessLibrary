// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Wrappers/Core/GLibGenericPlatformMisc.h"
#include "GLibModuleDescriptor.generated.h"


struct FModuleDescriptor;


UENUM(BlueprintType)
enum class EGLibHostType : uint8
{
	Runtime = 0,
	RuntimeNoCommandlet = 1,
	RuntimeAndProgram = 2,
	CookedOnly = 3,
	UncookedOnly = 4,
	Developer = 5,
	DeveloperTool = 6,
	Editor = 7,
	EditorNoCommandlet = 8,
	EditorAndProgram = 9,
	Program = 10,
	ServerOnly = 11,
	ClientOnly = 12,
	ClientOnlyNoCommandlet = 13,
	Max = 14
};

UENUM(BlueprintType)
enum class EGLibLoadingPhase : uint8
{
	/** As soon as possible - in other words, uplugin files are loadable from a pak file (as well as right after PlatformFile is set up in case pak files aren't used) Used for plugins needed to read files (compression formats, etc) */
	EarliestPossible = 0,
	/** Loaded before the engine is fully initialized, immediately after the config system has been initialized.  Necessary only for very low-level hooks */
	PostConfigInit = 1,
	/** The first screen to be rendered after system splash screen */
	PostSplashScreen = 2,
	/** Loaded before coreUObject for setting up manual loading screens, used for our chunk patching system */
	PreEarlyLoadingScreen = 3,
	/** Loaded before the engine is fully initialized for modules that need to hook into the loading screen before it triggers */
	PreLoadingScreen = 4,
	/** Right before the default phase */
	PreDefault = 5,
	/** Loaded at the default loading point during startup (during engine init, after game modules are loaded.) */
	Default = 6,
	/** Right after the default phase */
	PostDefault = 7,
	/** After the engine has been initialized */
	PostEngineInit = 8,
	/** Do not automatically load this module */
	None = 9,
	Max = 10
};

/**
* Description of a loadable module.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibModuleDescriptor
{
	GENERATED_BODY()

public:
	/** Name of this module */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	FName Name;

	/** Usage type of module */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	EGLibHostType Type;

	/** When should the module be loaded during the startup sequence?  This is sort of an advanced setting. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	EGLibLoadingPhase LoadingPhase;

	/** List of allowed platforms */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<FString> PlatformAllowList;

	/** List of disallowed platforms */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<FString> PlatformDenyList;

	/** List of allowed targets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<EGLibBuildTargetType> TargetAllowList;

	/** List of disallowed targets */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<EGLibBuildTargetType> TargetDenyList;

	/** List of allowed target configurations */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<EGLibBuildConfiguration> TargetConfigurationAllowList;

	/** List of disallowed target configurations */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<EGLibBuildConfiguration> TargetConfigurationDenyList;

	/** List of allowed programs */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<FString> ProgramAllowList;

	/** List of disallowed programs */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<FString> ProgramDenyList;

	/** List of additional dependencies for building this module. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	TArray<FString> AdditionalDependencies;

	/** When true, an empty PlatformAllowList is interpeted as 'no platforms' with the expectation that explict platforms will be added in plugin extensions */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleDescriptor")
	bool bHasExplicitPlatforms;

public:
	/** Normal constructor */
	FGLibModuleDescriptor(const FName InName = NAME_None, EGLibHostType InType = EGLibHostType::Runtime, EGLibLoadingPhase InLoadingPhase = EGLibLoadingPhase::Default);
	FGLibModuleDescriptor(const FModuleDescriptor& InDescriptor);
	
	operator FModuleDescriptor() const;

	// TODO: GLIB-14
	/** Reads a descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// bool Read(const FJsonObject& Object, FText * OutFailReason = nullptr);

	/** Reads a descriptor from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// bool Read2(const FJsonObject& Object, FText& OutFailReason);

	/** Reads an array of modules from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static bool ReadArray(const FJsonObject& Object, const TCHAR * Name, TArray<FGLibModuleDescriptor>& OutModules, FText * OutFailReason = nullptr);

	/** Reads an array of modules from the given JSON object */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static bool ReadArray2(const FJsonObject& Object, const TCHAR * Name, TArray<FGLibModuleDescriptor>& OutModules, FText& OutFailReason);

	/** Writes a descriptor to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// void Write(TJsonWriter<>& Writer);

	/** Updates the given json object with values in this descriptor */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// void UpdateJson(FJsonObject& JsonObject);

	/** Writes an array of modules to JSON */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static void WriteArray(TJsonWriter<>& Writer, const TCHAR * ArrayName, const TArray<FGLibModuleDescriptor>& Modules);

	/** Updates an array of module descriptors in the specified JSON field (indexed by module name) */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static void UpdateArray(FJsonObject& JsonObject, const TCHAR * ArrayName, const TArray<FGLibModuleDescriptor>& Modules);

	/** Tests whether the module should be built for the given target */
	// UFUNCTION(BlueprintPure, Category = "GLib|ModuleDescriptor")
	// bool IsCompiledInConfiguration(const FString& Platform, EGLibBuildConfiguration Configuration, const FString& TargetName, EGLibBuildTargetType TargetType, bool bBuildDeveloperTools, bool bBuildRequiresCookedData);

	/** Tests whether the module should be built for the current engine configuration */
	// UFUNCTION(BlueprintPure, Category = "GLib|ModuleDescriptor")
	// bool IsCompiledInCurrentConfiguration();

	/** Tests whether the module should be loaded for the current engine configuration */
	// UFUNCTION(BlueprintPure, Category = "GLib|ModuleDescriptor")
	// bool IsLoadedInCurrentConfiguration();

	/** Loads all the modules for a given loading phase. Returns a map of module names to load errors */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static void LoadModulesForPhase(EGLibLoadingPhase LoadingPhase, const TArray<FGLibModuleDescriptor>& Modules, TMap<FName, EGLibModuleLoadResult>& ModuleLoadErrors);

	/** Checks that all modules are compatible with the current engine version. Returns false and appends a list of names to OutIncompatibleFiles if not. */
	// UFUNCTION(BlueprintCallable, Category = "GLib|ModuleDescriptor")
	// static bool CheckModuleCompatibility(const TArray<FGLibModuleDescriptor>& Modules, TArray<FString>& OutIncompatibleFiles);

};

/** Context information used when validating that source code is being placed in the correct place for a given module */
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYRUNTIME_API FGLibModuleContextInfo
{
	GENERATED_BODY()

public:
	/** Path to the Source folder of the module */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleContextInfo")
	FString ModuleSourcePath;

	/** Name of the module */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleContextInfo")
	FString ModuleName;

	/** Type of this module, eg, Runtime, Editor, etc */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|ModuleContextInfo")
	EGLibHostType ModuleType;

};

