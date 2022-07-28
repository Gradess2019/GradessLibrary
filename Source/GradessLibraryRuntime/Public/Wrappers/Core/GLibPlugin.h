// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"
#include "Modules/GLibModuleManager.h"
#include "UObject/Object.h"
#include "Wrappers/Projects/GLibPluginDescriptor.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

#include "GLibPlugin.generated.h"


/**
* Information about an enabled plugin.
*/
UCLASS(BlueprintType, Blueprintable)
class GRADESSLIBRARYRUNTIME_API UGLibPlugin : public UObject
{
	GENERATED_BODY()

public:
	static UGLibPlugin* CreateWrapper(const TSharedPtr<IPlugin>& InPlugin);
	static UGLibPlugin* CreateWrapper(const TSharedRef<IPlugin>& InPlugin);

	virtual void SetPlugin(const TSharedPtr<IPlugin>& InPlugin) { Plugin = InPlugin; }
	virtual void SetPlugin(const TSharedRef<IPlugin>& InPlugin) { Plugin = InPlugin; }
	
	virtual TSharedPtr<IPlugin> GetPlugin() { return Plugin; }

	operator TSharedPtr<IPlugin>() const { return Plugin; }
	operator TSharedRef<IPlugin>() const { return Plugin.ToSharedRef(); }

	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool IsValid();
	
	/**
	* Gets the plugin name.
	*
	* @return Name of the plugin.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetName();

	/**
	* Return plugin friendly name if available or the same name as GetName() otherwise.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetFriendlyName();

	/**
	* Get a path to the plugin's descriptor
	*
	* @return Path to the plugin's descriptor.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetDescriptorFileName();

	/**
	* Get a path to the plugin's directory.
	*
	* @return Path to the plugin's base directory.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetBaseDir();

	/**
	* Get a path to the plugin's content directory.
	*
	* @return Path to the plugin's content directory.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetContentDir();

	/**
	* Get the virtual root path for assets.
	*
	* @return The mounted root path for assets in this plugin's content folder; typically /PluginName/.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FString GetMountedAssetPath();
	
	/**
	* Gets the type of a plugin
	*
	* @return The plugin type
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual EGLibPluginType GetType();

	/**
	* Determines if the plugin is enabled.
	*
	* @return True if the plugin is currently enabled.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool IsEnabled();

	/**
	* Determines if the plugin is enabled by default.
	*
	* @return True if the plugin is currently enabled by default.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool IsEnabledByDefault(bool bAllowEnginePluginsEnabledByDefault);

	/**
	* Determines if the plugin is should be displayed in-editor for the user to enable/disable freely.
	*
	* @return True if the plugin should be hidden.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool IsHidden();

	/**
	* Determines if the plugin can contain content.
	*
	* @return True if the plugin can contain content.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool CanContainContent();

	/**
	* Determines if the plugin can contain Verse code.
	*
	* @return True if the plugin can contain Verse code.
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual bool CanContainVerse();

	/**
	* Returns the plugin's location
	*
	* @return Where the plugin was loaded from
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual EGLibPluginLoadedFrom GetLoadedFrom();

	/**
	* Gets the plugin's descriptor
	*
	* @return Reference to the plugin's descriptor
	*/
	UFUNCTION(BlueprintPure, Category = "GLib|Plugin")
	virtual FGLibPluginDescriptor GetDescriptor();

	/**
	* Updates the plugin's descriptor
	*
	* @param NewDescriptor The new plugin descriptor
	* @param OutFailReason The error message if the plugin's descriptor could not be updated
	* @return True if the descriptor was updated, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, Category = "GLib|Plugin")
	virtual bool UpdateDescriptor(const FGLibPluginDescriptor& NewDescriptor, FText& OutFailReason);

	// TODO: GLIB-14
	/**
	* Gets the cached plugin descriptor json
	*
	* @return Reference to the cached plugin descriptor json
	*/
	// UFUNCTION(BlueprintCallable, Category = "GLib|Plugin")
	// virtual const TSharedPtr<FJsonObject>& GetDescriptorJson();
	
protected:
	TSharedPtr<IPlugin> Plugin;
};