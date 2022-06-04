
#pragma once

#include "CoreMinimal.h"
#include "PluginUtils.h"

#include "Wrappers/Core/GLibPlugin.h"
#include "Wrappers/Projects/GLibModuleDescriptor.h"
#include "Wrappers/Projects/GLibPluginDescriptor.h"

#include "GLibPluginUtils.generated.h"


enum class EGLibPluginEnabledByDefault : uint8;
class IPlugin;
struct FGLibMountPluginParams;


/**
 * Parameters for creating a new plugin.
 */
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYEDITOR_API FGLibNewPluginParams
{
	GENERATED_BODY()

public:
	/** The author of this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	FString CreatedBy;

	/** Hyperlink for the author's website  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	FString CreatedByURL;

	/** A friendly name for this plugin. Set to the plugin name by default */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	FString FriendlyName;

	/** A description for this plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	FString Description;

	/** Path to plugin icon to copy in the plugin resources folder */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	FString PluginIconPath;

	/**
	* Folders containing template files to copy into the plugin folder (Required if bHasModules).
	* Occurrences of the string PLUGIN_NAME in the filename or file content will be replaced by the plugin name.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	TArray<FString> TemplateFolders;

	/** Marks this content as being in beta */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	bool bIsBetaVersion;

	/** Can this plugin contain content */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	bool bCanContainContent;

	/** Does this plugin have Source files? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	bool bHasModules;

	/**
	* When true, this plugin's modules will not be loaded automatically nor will it's content be mounted automatically.
	* It will load/mount when explicitly requested and LoadingPhases will be ignored.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	bool bExplicitelyLoaded;

	/** Whether this plugin should be enabled/disabled by default for any project. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	EGLibPluginEnabledByDefault EnabledByDefault;

	/** If this plugin has Source, what is the type of Source included (so it can potentially be excluded in the right builds) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	EGLibHostType ModuleDescriptorType;

	/** If this plugin has Source, when should the module be loaded (may need to be earlier than default if used in blueprints) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParams")
	EGLibLoadingPhase LoadingPhase;

	operator FPluginUtils::FNewPluginParams() const;

	FGLibNewPluginParams();
	FGLibNewPluginParams(const FPluginUtils::FNewPluginParams& Params);
};

/**
* Parameters for creating a new plugin.
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYEDITOR_API FGLibNewPluginParamsWithDescriptor
{
	GENERATED_BODY()

public:
	/** The description of the plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParamsWithDescriptor")
	FGLibPluginDescriptor Descriptor;

	/** Path to plugin icon to copy in the plugin resources folder */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParamsWithDescriptor")
	FString PluginIconPath;

	/**
	* Folders containing template files to copy into the plugin folder (Required if Descriptor.Modules is not empty).
	* Occurrences of the string PLUGIN_NAME in the filename or file content will be replaced by the plugin name.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|NewPluginParamsWithDescriptor")
	TArray<FString> TemplateFolders;

	operator FPluginUtils::FNewPluginParamsWithDescriptor() const;

	FGLibNewPluginParamsWithDescriptor();
	FGLibNewPluginParamsWithDescriptor(const FPluginUtils::FNewPluginParamsWithDescriptor& Params);
};

/**
* Parameters for loading/mounting a plugin
*/
USTRUCT(BlueprintType, Blueprintable)
struct GRADESSLIBRARYEDITOR_API FGLibLoadPluginParams
{
	GENERATED_BODY()

public:
	/** Whether to synchronously scan all assets in the plugin */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	bool bSynchronousAssetsScan;

	/** Whether to select the plugin Content folder (if any) in the content browser */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	bool bSelectInContentBrowser;

	/** Whether to enable the plugin in the current project config */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	bool bEnablePluginInProject;

	/**
	* Whether to update the project additional plugin directories (persistently saved in uproject file)
	* if the plugin location is not under the engine or project plugin folder
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	bool bUpdateProjectPluginSearchPath;

	/** Outputs whether the plugin was already loaded */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	bool bOutAlreadyLoaded;

	/** Outputs the reason the plugin loading failed (if applicable) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GLib|LoadPluginParams")
	FText OutFailReason;

	operator FPluginUtils::FLoadPluginParams() const;

	FGLibLoadPluginParams();
	FGLibLoadPluginParams(const FPluginUtils::FLoadPluginParams& InParams);
};

UCLASS(BlueprintType, Blueprintable)
class GRADESSLIBRARYEDITOR_API UGLibPluginUtils : public UObject
{
	GENERATED_BODY()

public:
	/**
	* Returns the plugin folder.
	* @param PluginLocation directory that contains the plugin folder
	* @param PluginName name of the plugin
	* @param bFullPath ensures a full path is returned
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static FString GetPluginFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath = false);

	/**
	* Returns the uplugin file path.
	* @param PluginLocation directory that contains the plugin folder
	* @param PluginName name of the plugin
	* @param bFullPath ensures a full path is returned
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static FString GetPluginFilePath(const FString& PluginLocation, const FString& PluginName, bool bFullPath = false);

	/**
	* Returns the plugin Content folder.
	* @param PluginLocation directory that contains the plugin folder
	* @param PluginName name of the plugin
	* @param bFullPath ensures a full path is returned
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static FString GetPluginContentFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath = false);

	/**
	* Returns the plugin Resources folder.
	* @param PluginLocation directory that contains the plugin folder
	* @param PluginName name of the plugin
	* @param bFullPath ensures a full path is returned
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static FString GetPluginResourcesFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath = false);

	/**
	* Helper to create and load a new plugin
	* @param PluginName Plugin name
	* @param PluginLocation Directory that contains the plugin folder
	* @param CreationParams Plugin creation parameters
	* @param MountParams Plugin loading parameters
	* @return The newly created plugin. If something goes wrong during the creation process, the plugin folder gets deleted and null is returned.
	* @note MountParams.OutFailReason outputs the reason the plugin creation or loading failed (if applicable)
	* @note Will fail if the plugin already exists
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* CreateAndLoadNewPlugin(const FString& PluginName, const FString& PluginLocation, const FGLibNewPluginParams& CreationParams, UPARAM(ref) FGLibLoadPluginParams& LoadParams);

	/**
	* Helper to create and load a new plugin
	* @param PluginName Plugin name
	* @param PluginLocation Directory that contains the plugin folder
	* @param CreationParams Plugin creation parameters
	* @param LoadParams Plugin loading parameters
	* @return The newly created plugin. If something goes wrong during the creation process, the plugin folder gets deleted and null is returned.
	* @note MountParams.OutFailReason outputs the reason the plugin creation or loading failed (if applicable)
	* @note Will fail if the plugin already exists
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* CreateAndLoadNewPluginWithDescriptor(const FString& PluginName, const FString& PluginLocation, const FGLibNewPluginParamsWithDescriptor& CreationParams, UPARAM(ref) FGLibLoadPluginParams& LoadParams);

	/**
	* Load/mount the specified plugin
	* @param PluginName Plugin name
	* @param PluginLocation Directory that contains the plugin folder
	* @param LoadParams Plugin loading parameters
	* @return The loaded plugin or null on failure
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* LoadPluginByNameWithParams(const FString& PluginName, const FString& PluginLocation, UPARAM(ref) FGLibLoadPluginParams& LoadParams);

	/**
	* Load/mount the specified plugin
	* @param PluginName Plugin name
	* @param PluginLocation Directory that contains the plugin folder
	* @return The loaded plugin or null on failure
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* LoadPluginByName(const FString& PluginName, const FString& PluginLocation);

	/**
	* Load/mount the specified plugin
	* @param PluginFileName Plugin descriptor file path
	* @param LoadParams Plugin loading parameters
	* @return The loaded plugin or null on failure
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* LoadPluginByPathWithParams(const FString& PluginFileName, UPARAM(ref) FGLibLoadPluginParams& LoadParams);

	/**
	* Load/mount the specified plugin
	* @param PluginFileName Plugin descriptor file path
	* @return The loaded plugin or null on failure
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* LoadPluginByPath(const FString& PluginFileName);

	/**
	* Finds a loaded plugin from a plugin descriptor file path
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static UGLibPlugin* FindLoadedPlugin(const FString& PluginDescriptorFileName);

	/**
	* Unload assets from the specified plugin and unmount it
	* @note Only works on content-only plugins; plugins with code modules cannot be safely unloaded
	* @warning Dirty assets that need to be saved will be unloaded anyway
	* @param Plugin Plugin to unload
	* @param OutFailReason Outputs the reason of the failure if any
	* @return Whether the plugin was successfully unloaded
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool UnloadPlugin(UGLibPlugin* Plugin, FText& OutFailReason);

	/**
	* Unload assets from the specified plugin and unmount it
	* @note Only works on content-only plugins; plugins with code modules cannot be safely unloaded
	* @warning Dirty assets that need to be saved will be unloaded anyway
	* @param PluginName Name of the plugin to unload
	* @param OutFailReason Outputs the reason of the failure if any
	* @return Whether the plugin was successfully unloaded
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool UnloadPluginByName(const FString& PluginName, FText& OutFailReason);

	/**
	* Unload assets from the specified plugins and unmount them
	* @note Only works on content-only plugins; plugins with code modules cannot be safely unloaded
	* @warning Dirty assets that need to be saved will be unloaded anyway
	* @param Plugins Plugins to unload
	* @param OutFailReason Outputs the reason of the failure if any
	* @return Whether all plugins were successfully unloaded
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool UnloadPlugins(const TArray<UGLibPlugin*> Plugins, FText& OutFailReason);

	/**
	* Unload assets from the specified plugins and unmount them
	* @note Only works on content-only plugins; plugins with code modules cannot be safely unloaded
	* @warning Dirty assets that need to be saved will be unloaded anyway
	* @param PluginNames Names of the plugins to unload
	* @param OutFailReason Outputs the reason of the failure if any
	* @return Whether all plugins were successfully unloaded
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool UnloadPluginsByName(const TArray<FString>& PluginNames, FText& OutFailReason);

	/**
	* Adds a directory to the list of paths that are recursively searched for plugins,
	* if that directory isn't already under the search paths.
	* @param Dir Directory to add (doesn't have to be an absolute or normalized path)
	* @param bRefreshPlugins Whether to refresh plugins if the search path list gets modified
	* @param bUpdateProjectFile Whether to update the project additional plugin directories (persistently saved in uproject file) if needed
	* @return Whether the plugin search path was modified
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool AddToPluginSearchPathIfNeeded(const FString& Dir, bool bRefreshPlugins = false, bool bUpdateProjectFile = false);

	/**
	* Validate that the plugin name is valid, that the name isn't already used by a registered plugin
	* and optionally that there isn't an unregistered plugin with that name that exists at the specified location.
	* @param PluginName Plugin name
	* @param PluginLocation Optional directory in which to look for a plugin that might not be registered
	* @param FailReason Optional output text describing why the validation failed
	* @return
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool ValidateNewPluginNameAndLocation(const FString& PluginName, FText& FailReason, const FString& PluginLocation = TEXT(""));

	/**
	* Returns whether the specified plugin name is valid, regardless of whether it's already used
	* @param PluginName Plugin name
	* @param FailReason Optional output text specifying what is wrong with the plugin name
	* @param PluginTermReplacement If set, replaces the term "plugin" in the fail reason message
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|PluginUtils")
	static bool IsValidPluginName(const FString& PluginName, FText& FailReason, const FText& PluginTermReplacement = FText());

};

