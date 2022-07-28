#pragma once


#include "CoreMinimal.h"

#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"

#include "Interfaces/IPluginManager.h"

#include "UObject/Object.h"
#include "Wrappers/Projects/GLibModuleDescriptor.h"
#include "Wrappers/Projects/GLibProjectDescriptor.h"

#include "GLibPluginManager.generated.h"


class UGLibPlugin;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGLibUpdatePackageLocalizationCacheDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibNewPluginMountedEvent, UGLibPlugin*, Plugin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGLibRegisterMountPointDelegate, const FString&, RootContentPath,  const FString&, DirectoryName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGLibOnLoadingPhaseComplete, EGLibLoadingPhase, LoadingPhase, bool, bSuccess);


/**
* Enum for the type of a plugin
*/
UENUM(BlueprintType)
enum class EGLibPluginType : uint8
{
	/** Plugin is built-in to the engine */
	Engine = 0,
	/** Standard enterprise plugin */
	Enterprise = 1,
	/** Project-specific plugin, stored within a game project directory */
	Project = 2,
	/** Plugin found in an external directory (found in an AdditionalPluginDirectory listed in the project file, or referenced on the command line) */
	External = 3,
	/** Project-specific mod plugin */
	Mod = 4,
	
	None = UINT8_MAX - 1,
	Max = UINT8_MAX,
};


/**
* PluginManager manages available code and content extensions (both loaded and not loaded).
*/
UCLASS(BlueprintType, Blueprintable)
class GRADESSLIBRARYRUNTIME_API UGLibPluginManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GLib|PluginManager")
	FGLibOnLoadingPhaseComplete OnLoadingPhaseComplete;

	/**
	* Event signature for being notified that a new plugin has been created
	*/
	UPROPERTY(BlueprintAssignable, Category = "GLib|PluginManager")
	FGLibNewPluginMountedEvent OnNewPluginCreated;

	/**
	* Event for being notified that a new plugin has been mounted
	*/
	UPROPERTY(BlueprintAssignable, Category = "GLib|PluginManager")
	FGLibNewPluginMountedEvent OnNewPluginMounted;

	/**
	* Event for being notified that a plugin has been edited
	*/
	UPROPERTY(BlueprintAssignable, Category = "GLib|PluginManager")
	FGLibNewPluginMountedEvent OnPluginEdited;

public:
	UGLibPluginManager();
	
	/**
	* Updates the list of plugins.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual void RefreshPluginsList();

	/**
	* Adds a single plugin to the list of plugins. Faster than refreshing all plugins with RefreshPluginsList() when you only want to add one. Does nothing if already in the list.
	*
	* @return True if the plugin was added or already in the list. False if it failed to load.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool AddToPluginsList(const FString& PluginFilename, FText& OutFailReason);

	/**
	* Loads all plug-ins
	*
	* @param	LoadingPhase	Which loading phase we're loading plug-in modules from.  Only modules that are configured to be
	*							loaded at the specified loading phase will be loaded during this call.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool LoadModulesForEnabledPlugins(const EGLibLoadingPhase LoadingPhase);

	/** Returns the highest loading phase that has so far completed */
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual EGLibLoadingPhase GetLastCompletedLoadingPhase();

	/**
	* Get the localization paths for all enabled plugins.
	*
	* @param	OutLocResPaths	Array to populate with the localization paths for all enabled plugins.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetLocalizationPathsForEnabledPlugins(TArray<FString>& OutLocResPaths);

	/**
	* Checks if all the required plug-ins are available. If not, will present an error dialog the first time a plug-in is loaded or this function is called.
	*
	* @returns true if all the required plug-ins are available.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool AreRequiredPluginsAvailable();

	/**
	* Checks whether modules for the enabled plug-ins are up to date.
	*
	* @param OutIncompatibleModules Array to receive a list of incompatible module names.
	* @param OutIncompatibleEngineModules Array to receive a list of incompatible engine module names.
	* @returns true if the enabled plug-in modules are up to date.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool CheckModuleCompatibility(TArray<FString>& OutIncompatibleModules, TArray<FString>& OutIncompatibleEngineModules);

	/**
	* Finds information for an enabled plugin.
	*
	* @return	 Pointer to the plugin's information, or nullptr.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual UGLibPlugin* FindPlugin(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual UGLibPlugin* FindPluginFromPath(const FString& PluginPath);

	/**
	* Gets an array of all the enabled plugins.
	*
	* @return	Array of the enabled plugins.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetEnabledPlugins(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Gets an array of all enabled plugins that can have content.
	*
	* @return	Array of plugins with IsEnabled() and CanContainContent() both true.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetEnabledPluginsWithContent(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Gets an array of all enabled plugins that can have Verse code.
	*
	* @return	Array of plugins with IsEnabled() and CanContainVerse() both true.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetEnabledPluginsWithVerse(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Gets an array of all enabled plugins that can have content or Verse code.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetEnabledPluginsWithContentOrVerse(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Gets an array of all the discovered plugins.
	*
	* @return	Array of the discovered plugins.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetDiscoveredPlugins(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Stores the specified path, utilizing it in future search passes when
	* searching for available plugins. Optionally refreshes the manager after
	* the new path has been added.
	*
	* @param  ExtraDiscoveryPath	The path you want searched for additional plugins.
	* @param  bRefresh				Signals the function to refresh the plugin database after the new path has been added
	* @return Whether the plugin search path was modified
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool AddPluginSearchPath(const FString& ExtraDiscoveryPath, bool bRefresh = true);

	/**
	* Returns the list of extra directories that are recursively searched for plugins (aside from the engine and project plugin directories).
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual TSet<FString> GetAdditionalPluginSearchPaths();

	/**
	* Gets an array of plugins that loaded their own content pak file
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual void GetPluginsWithPakFile(TArray<UGLibPlugin*>& OutPlugins);

	/**
	* Marks a newly created plugin as enabled, mounts its content and tries to load its modules
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual void MountNewlyCreatedPlugin(const FString& PluginName);

	/**
	* Marks an explicitly loaded plugin as enabled, mounts its content and tries to load its modules.
	* These plugins are not loaded implicitly, but instead wait for this function to be called.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual void MountExplicitlyLoadedPlugin(const FString& PluginName);

	/**
	* Marks an explicitly loaded plugin as disabled, unmounts its content (does not work on plugins with compiled modules).
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool UnmountExplicitlyLoadedPlugin(const FString& PluginName, FText& OutReason);

	/**
	* Does a reverse lookup to try to figure out what the UObject package name is for a plugin
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|PluginManager")
	virtual FName PackageNameFromModuleName(FName ModuleName);

	/**
	* Determines if a content-only project requires a temporary target due to having a plugin enabled
	*
	* @param ProjectDescriptor The project being built
	* @param Platform The platform the target is being built for
	* @param Configuration The configuration being built
	* @param TargetType The type of target being built
	* @param OutReason If a temporary target is required, receives a message indicating why
	* @return True if the project requires a temp target to be generated
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager")
	virtual bool RequiresTempTargetForCodePlugin(const FGLibProjectDescriptor& ProjectDescriptor, const FString& Platform, EGLibBuildConfiguration Configuration, EGLibBuildTargetType TargetType, FText& OutReason);

	UFUNCTION()
	virtual bool IsValidManager();
	
	/**
	* Static: Access singleton instance.
	*
	* @return	Reference to the singleton object.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginManager", meta = (DisplayName = "Get Plugin Manager"))
	static UGLibPluginManager* Get();

protected:
	IPluginManager* PluginManager;
	static UGLibPluginManager* Instance;
	
};