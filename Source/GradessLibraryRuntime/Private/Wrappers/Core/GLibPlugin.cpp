// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Core/GLibPlugin.h"
#include "UObject/UObjectGlobals.h"
#include "Wrappers/GLibConversions.h"
#include "Wrappers/Projects/GLibPluginDescriptor.h"

UGLibPlugin* UGLibPlugin::CreateWrapper(const TSharedPtr<IPlugin>& InPlugin)
{
	return CreateWrapper(InPlugin.ToSharedRef());
}

UGLibPlugin* UGLibPlugin::CreateWrapper(const TSharedRef<IPlugin>& InPlugin)
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
	return IsValid() ? Plugin->GetName() : FString();
}

FString UGLibPlugin::GetFriendlyName()
{
	return IsValid() ? Plugin->GetFriendlyName() : FString();
}

FString UGLibPlugin::GetDescriptorFileName()
{
	return IsValid() ? Plugin->GetDescriptorFileName() : FString();
}

FString UGLibPlugin::GetBaseDir()
{
	return IsValid() ? Plugin->GetBaseDir() : FString();
}

FString UGLibPlugin::GetContentDir()
{
	return IsValid() ? Plugin->GetContentDir() : FString();
}

FString UGLibPlugin::GetMountedAssetPath()
{
	return IsValid() ? Plugin->GetMountedAssetPath() : FString();
}

bool UGLibPlugin::IsEnabled()
{
	return IsValid() ? Plugin->IsEnabled() : false;
}

bool UGLibPlugin::IsEnabledByDefault(bool bAllowEnginePluginsEnabledByDefault)
{
	return IsValid() ? Plugin->IsEnabledByDefault(bAllowEnginePluginsEnabledByDefault) : false;
}

bool UGLibPlugin::IsHidden()
{
	return IsValid() ? Plugin->IsHidden() : false;
}

bool UGLibPlugin::CanContainContent()
{
	return IsValid() ? Plugin->CanContainContent() : false;
}

bool UGLibPlugin::CanContainVerse()
{
	return IsValid() ? Plugin->CanContainVerse() : false;
}

EGLibPluginLoadedFrom UGLibPlugin::GetLoadedFrom()
{
	return IsValid() ? GLibConversions::ToBlueprintable<EGLibPluginLoadedFrom>(Plugin->GetLoadedFrom()) : EGLibPluginLoadedFrom::Engine;
}

FGLibPluginDescriptor UGLibPlugin::GetDescriptor()
{
	return IsValid() ? Plugin->GetDescriptor() : FGLibPluginDescriptor();
}

bool UGLibPlugin::UpdateDescriptor(const FGLibPluginDescriptor& NewDescriptor, FText& OutFailReason)
{
	return IsValid() ? Plugin->UpdateDescriptor(NewDescriptor, OutFailReason) : false;
}
