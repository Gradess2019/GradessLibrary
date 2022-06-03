// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/PluginUtils/GLibPluginUtils.h"
#include "PluginUtils.h"
#include "Wrappers/GLibConversions.h"
#include "Wrappers/Core/GLibPlugin.h"

FGLibNewPluginParams::operator FPluginUtils::FNewPluginParams() const
{
	FPluginUtils::FNewPluginParams ConvertedParams;
	ConvertedParams.CreatedBy = CreatedBy;
	ConvertedParams.CreatedByURL = CreatedByURL;
	ConvertedParams.FriendlyName = FriendlyName;
	ConvertedParams.Description = Description;
	ConvertedParams.PluginIconPath = PluginIconPath;
	ConvertedParams.TemplateFolders = TemplateFolders;
	ConvertedParams.bIsBetaVersion = bIsBetaVersion;
	ConvertedParams.bCanContainContent = bCanContainContent;
	ConvertedParams.bHasModules = bHasModules;
	ConvertedParams.bExplicitelyLoaded = bExplicitelyLoaded;
	ConvertedParams.EnabledByDefault = static_cast<EPluginEnabledByDefault>(EnabledByDefault);
	ConvertedParams.ModuleDescriptorType = GLibConversions::ToNative<EHostType::Type>(ModuleDescriptorType);
	ConvertedParams.LoadingPhase = GLibConversions::ToNative<ELoadingPhase::Type>(LoadingPhase);
	return ConvertedParams;
}

FGLibNewPluginParams::FGLibNewPluginParams()
{
	CreatedBy = "";
	CreatedByURL = "";
	FriendlyName = "";
	Description = "";
	PluginIconPath = "";
	TemplateFolders.Add("");
	bIsBetaVersion = false;
	bCanContainContent = false;
	bHasModules = false;
	bExplicitelyLoaded = false;
	EnabledByDefault = static_cast<EGLibPluginEnabledByDefault>(EPluginEnabledByDefault::Unspecified);
	ModuleDescriptorType = EGLibHostType::Runtime;
	LoadingPhase = EGLibLoadingPhase::Default;
}

FGLibNewPluginParams::FGLibNewPluginParams(const FPluginUtils::FNewPluginParams& Params)
{
	CreatedBy = Params.CreatedBy;
	CreatedByURL = Params.CreatedByURL;
	FriendlyName = Params.FriendlyName;
	Description = Params.Description;
	PluginIconPath = Params.PluginIconPath;
	TemplateFolders = Params.TemplateFolders;
	bIsBetaVersion = Params.bIsBetaVersion;
	bCanContainContent = Params.bCanContainContent;
	bHasModules = Params.bHasModules;
	bExplicitelyLoaded = Params.bExplicitelyLoaded;
	EnabledByDefault = static_cast<EGLibPluginEnabledByDefault>(Params.EnabledByDefault);
	ModuleDescriptorType = GLibConversions::ToBlueprintable<EGLibHostType>(Params.ModuleDescriptorType);
	LoadingPhase = GLibConversions::ToBlueprintable<EGLibLoadingPhase>(Params.LoadingPhase);
}

FGLibNewPluginParamsWithDescriptor::operator FPluginUtils::FNewPluginParamsWithDescriptor() const
{
	FPluginUtils::FNewPluginParamsWithDescriptor ConvertedParams;
	ConvertedParams.Descriptor = Descriptor;
	ConvertedParams.PluginIconPath = PluginIconPath;
	ConvertedParams.TemplateFolders = TemplateFolders;
	return ConvertedParams;
}

FGLibNewPluginParamsWithDescriptor::FGLibNewPluginParamsWithDescriptor()
{
	Descriptor = FGLibPluginDescriptor();
	PluginIconPath = "";
	TemplateFolders = TArray<FString>();
}

FGLibNewPluginParamsWithDescriptor::FGLibNewPluginParamsWithDescriptor(const FPluginUtils::FNewPluginParamsWithDescriptor& Params)
{
	Descriptor = Params.Descriptor;
	PluginIconPath = Params.PluginIconPath;
	TemplateFolders = Params.TemplateFolders;
}

FGLibLoadPluginParams::operator FPluginUtils::FLoadPluginParams() const
{
	auto ConvertedParams = FPluginUtils::FLoadPluginParams();
	ConvertedParams.bSynchronousAssetsScan = bSynchronousAssetsScan;
	ConvertedParams.bSelectInContentBrowser = bSelectInContentBrowser;
	ConvertedParams.bEnablePluginInProject = bEnablePluginInProject;
	ConvertedParams.bUpdateProjectPluginSearchPath = bUpdateProjectPluginSearchPath;
	ConvertedParams.bOutAlreadyLoaded = bOutAlreadyLoaded;

	// TODO: check if this is correct
	Move(*ConvertedParams.OutFailReason, OutFailReason);
	return ConvertedParams;
}

FGLibLoadPluginParams::FGLibLoadPluginParams()
{
	bSynchronousAssetsScan = false;
	bSelectInContentBrowser = false;
	bEnablePluginInProject = false;
	bUpdateProjectPluginSearchPath = false;
	bOutAlreadyLoaded = false;
	OutFailReason = FText();
}

FGLibLoadPluginParams::FGLibLoadPluginParams(const FPluginUtils::FLoadPluginParams& InParams)
{
	bSynchronousAssetsScan = InParams.bSynchronousAssetsScan;
	bSelectInContentBrowser = InParams.bSelectInContentBrowser;
	bEnablePluginInProject = InParams.bEnablePluginInProject;
	bUpdateProjectPluginSearchPath = InParams.bUpdateProjectPluginSearchPath;
	bOutAlreadyLoaded = InParams.bOutAlreadyLoaded;
	OutFailReason = *InParams.OutFailReason;
}

FString UGLibPluginUtils::GetPluginFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath)
{
	return FPluginUtils::GetPluginFolder(PluginLocation, PluginName, bFullPath);
}

FString UGLibPluginUtils::GetPluginFilePath(const FString& PluginLocation, const FString& PluginName, bool bFullPath)
{
	return FPluginUtils::GetPluginFilePath(PluginLocation, PluginName, bFullPath);
}

FString UGLibPluginUtils::GetPluginContentFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath)
{
	return FPluginUtils::GetPluginContentFolder(PluginLocation, PluginName, bFullPath);
}

FString UGLibPluginUtils::GetPluginResourcesFolder(const FString& PluginLocation, const FString& PluginName, bool bFullPath)
{
	return FPluginUtils::GetPluginResourcesFolder(PluginLocation, PluginName, bFullPath);
}

UGLibPlugin* UGLibPluginUtils::CreateAndLoadNewPlugin(const FString& PluginName, const FString& PluginLocation, const FGLibNewPluginParams& CreationParams, FGLibLoadPluginParams& LoadParams)
{
	FPluginUtils::FLoadPluginParams NativeLoadParams = LoadParams;
	const auto Plugin = FPluginUtils::CreateAndLoadNewPlugin(PluginName, PluginLocation, CreationParams, NativeLoadParams);

	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::CreateAndLoadNewPluginWithDescriptor(const FString& PluginName, const FString& PluginLocation, const FGLibNewPluginParamsWithDescriptor& CreationParams, FGLibLoadPluginParams& LoadParams)
{
	FPluginUtils::FLoadPluginParams NativeLoadParams = LoadParams;
	const auto Plugin = FPluginUtils::CreateAndLoadNewPlugin(PluginName, PluginLocation, CreationParams, NativeLoadParams);

	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::LoadPluginByNameWithParams(const FString& PluginName, const FString& PluginLocation, FGLibLoadPluginParams& LoadParams)
{
	FPluginUtils::FLoadPluginParams NativeLoadParams = LoadParams;
	const auto Plugin = FPluginUtils::LoadPlugin(PluginName, PluginLocation, NativeLoadParams);
	
	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::LoadPluginByName(const FString& PluginName, const FString& PluginLocation)
{
	const auto Plugin = FPluginUtils::LoadPlugin(PluginName, PluginLocation);

	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::LoadPluginByPathWithParams(const FString& PluginFileName, FGLibLoadPluginParams& LoadParams)
{
	FPluginUtils::FLoadPluginParams NativeLoadParams = LoadParams;
	const auto Plugin = FPluginUtils::LoadPlugin(PluginFileName, NativeLoadParams);

	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::LoadPluginByPath(const FString& PluginFileName)
{
	const auto Plugin = FPluginUtils::LoadPlugin(PluginFileName);

	return UGLibPlugin::CreateWrapper(Plugin);
}

UGLibPlugin* UGLibPluginUtils::FindLoadedPlugin(const FString& PluginDescriptorFileName)
{
	const auto Plugin = FPluginUtils::FindLoadedPlugin(PluginDescriptorFileName);

	return UGLibPlugin::CreateWrapper(Plugin);
}

bool UGLibPluginUtils::UnloadPlugin(UGLibPlugin* Plugin, FText& OutFailReason)
{
	if (!IsValid(Plugin))
	{
		OutFailReason = NSLOCTEXT("GLibPluginUtils", "PluginNotFound", "Plugin not found");
		return false;
	}
	return FPluginUtils::UnloadPlugin(Plugin->GetPlugin().ToSharedRef(), &OutFailReason);
}

bool UGLibPluginUtils::UnloadPluginByName(const FString& PluginName, FText& OutFailReason)
{
	return FPluginUtils::UnloadPlugin(PluginName, &OutFailReason);
}

bool UGLibPluginUtils::UnloadPlugins(const TArray<UGLibPlugin*> Plugins, FText& OutFailReason)
{
	TArray<TSharedRef<IPlugin>> NativePlugins;
	for (auto Plugin : Plugins)
	{
		if (!IsValid(Plugin) || !Plugin->IsValid()) { continue; }
		
		NativePlugins.Add(*Plugin);
	}
	
	return FPluginUtils::UnloadPlugins(NativePlugins, &OutFailReason);
}

bool UGLibPluginUtils::UnloadPluginsByName(const TArray<FString>& PluginNames, FText& OutFailReason)
{
	return FPluginUtils::UnloadPlugins(PluginNames, &OutFailReason);
}

bool UGLibPluginUtils::AddToPluginSearchPathIfNeeded(const FString& Dir, bool bRefreshPlugins, bool bUpdateProjectFile)
{
	return FPluginUtils::AddToPluginSearchPathIfNeeded(Dir, bRefreshPlugins, bUpdateProjectFile);
}

bool UGLibPluginUtils::ValidateNewPluginNameAndLocation(const FString& PluginName, FText& FailReason, const FString& PluginLocation)
{
	return FPluginUtils::ValidateNewPluginNameAndLocation(PluginName, PluginLocation, &FailReason);
}

bool UGLibPluginUtils::IsValidPluginName(const FString& PluginName, FText& FailReason, const FText& PluginTermReplacement)
{
	return FPluginUtils::IsValidPluginName(PluginName, &FailReason, &PluginTermReplacement);
}
