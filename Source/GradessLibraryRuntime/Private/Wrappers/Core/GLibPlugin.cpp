// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Core/GLibPlugin.h"
#include "UObject/UObjectGlobals.h"

#include "Wrappers/GLibConversions.h"
#include "Wrappers/Projects/GLibPluginDescriptor.h"

UGLibPlugin* UGLibPlugin::CreateWrapper(TSharedPtr<IPlugin> InPlugin)
{
	const auto PluginWrapper = NewObject<UGLibPlugin>();
	PluginWrapper->SetPlugin(InPlugin);
	return PluginWrapper;
}

bool UGLibPlugin::IsValid()
{
	return Plugin.IsValid();
}

FString UGLibPlugin::GetName()
{
	return Plugin.IsValid() ? Plugin->GetName() : FString();
}

FString UGLibPlugin::GetFriendlyName()
{
	return Plugin.IsValid() ? Plugin->GetFriendlyName() : FString();
}

FString UGLibPlugin::GetDescriptorFileName()
{
	return Plugin.IsValid() ? Plugin->GetDescriptorFileName() : FString();
}

FString UGLibPlugin::GetBaseDir()
{
	return Plugin.IsValid() ? Plugin->GetBaseDir() : FString();
}

FString UGLibPlugin::GetContentDir()
{
	return Plugin.IsValid() ? Plugin->GetContentDir() : FString();
}

FString UGLibPlugin::GetMountedAssetPath()
{
	return Plugin.IsValid() ? Plugin->GetMountedAssetPath() : FString();
}

bool UGLibPlugin::IsEnabled()
{
	return Plugin.IsValid() ? Plugin->IsEnabled() : false;
}

bool UGLibPlugin::IsEnabledByDefault(bool bAllowEnginePluginsEnabledByDefault)
{
	return Plugin.IsValid() ? Plugin->IsEnabledByDefault(bAllowEnginePluginsEnabledByDefault) : false;
}

bool UGLibPlugin::IsHidden()
{
	return Plugin.IsValid() ? Plugin->IsHidden() : false;
}

bool UGLibPlugin::CanContainContent()
{
	return Plugin.IsValid() ? Plugin->CanContainContent() : false;
}

bool UGLibPlugin::CanContainVerse()
{
	return Plugin.IsValid() ? Plugin->CanContainVerse() : false;
}

EGLibPluginLoadedFrom UGLibPlugin::GetLoadedFrom()
{
	return Plugin.IsValid() ? GLibConversions::ToBlueprintable<EGLibPluginLoadedFrom>(Plugin->GetLoadedFrom()) : EGLibPluginLoadedFrom::Engine;
}

FGLibPluginDescriptor UGLibPlugin::GetDescriptor()
{
	return Plugin.IsValid() ? Plugin->GetDescriptor() : FGLibPluginDescriptor();
}

bool UGLibPlugin::UpdateDescriptor(const FGLibPluginDescriptor& NewDescriptor, FText& OutFailReason)
{
	return Plugin.IsValid() ? Plugin->UpdateDescriptor(NewDescriptor, OutFailReason) : false;
}
