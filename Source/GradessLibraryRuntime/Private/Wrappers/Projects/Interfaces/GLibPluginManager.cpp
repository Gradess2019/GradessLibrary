// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"
#include "Interfaces/IPluginManager.h"
#include "Wrappers/GLibConversions.h"
#include "Wrappers/Core/GLibPlugin.h"

TSharedPtr<UGLibPluginManager> UGLibPluginManager::Instance = nullptr;

UGLibPluginManager::UGLibPluginManager()
{
	PluginManager = MakeShareable(&IPluginManager::Get());
	if (!PluginManager.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("UGLibPluginManager::UGLibPluginManager: PluginManager is not valid"));
		return;
	}

	PluginManager->OnLoadingPhaseComplete().AddLambda([this](ELoadingPhase::Type LoadingPhase, bool bSuccess)
	{
		OnLoadingPhaseComplete.Broadcast(GLibConversions::ToBlueprintable<EGLibLoadingPhase>(LoadingPhase), bSuccess);
	});

	PluginManager->OnNewPluginCreated().AddLambda([this](IPlugin& PluginInfo)
	{
		OnNewPluginCreated.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(&PluginInfo));
	});

	PluginManager->OnNewPluginMounted().AddLambda([this](IPlugin& PluginInfo)
	{
		OnNewPluginMounted.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(&PluginInfo));
	});

	PluginManager->OnPluginEdited().AddLambda([this](IPlugin& PluginInfo)
	{
		OnPluginEdited.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(&PluginInfo));
	});
}

void UGLibPluginManager::RefreshPluginsList()
{
	if (!PluginManager.IsValid()) { return; }
	PluginManager->RefreshPluginsList();
}

bool UGLibPluginManager::AddToPluginsList(const FString& PluginFilename, FText& OutFailReason)
{
	return PluginManager.IsValid() ? PluginManager->AddToPluginsList(PluginFilename, &OutFailReason) : false;
}

bool UGLibPluginManager::LoadModulesForEnabledPlugins(const EGLibLoadingPhase LoadingPhase)
{
	return PluginManager.IsValid() ? PluginManager->LoadModulesForEnabledPlugins(GLibConversions::ToNative<ELoadingPhase::Type>(LoadingPhase)) : false;
}

EGLibLoadingPhase UGLibPluginManager::GetLastCompletedLoadingPhase()
{
	return PluginManager.IsValid() ? GLibConversions::ToBlueprintable<EGLibLoadingPhase>(PluginManager->GetLastCompletedLoadingPhase()) : EGLibLoadingPhase::None;
}

void UGLibPluginManager::GetLocalizationPathsForEnabledPlugins(TArray<FString>& OutLocResPaths)
{
	if (!PluginManager.IsValid()) { return; }
	PluginManager->GetLocalizationPathsForEnabledPlugins(OutLocResPaths);
}

bool UGLibPluginManager::AreRequiredPluginsAvailable()
{
	return PluginManager.IsValid() ? PluginManager->AreRequiredPluginsAvailable() : false;
}

bool UGLibPluginManager::CheckModuleCompatibility(TArray<FString>& OutIncompatibleModules, TArray<FString>& OutIncompatibleEngineModules)
{
	return PluginManager.IsValid() ? PluginManager->CheckModuleCompatibility(OutIncompatibleModules, OutIncompatibleEngineModules) : false;
}

UGLibPlugin* UGLibPluginManager::FindPlugin(const FString& Name)
{
	return PluginManager.IsValid() ? GLibConversions::ToBlueprintable<UGLibPlugin>(PluginManager->FindPlugin(*Name)) : nullptr;
}

UGLibPlugin* UGLibPluginManager::FindPluginFromPath(const FString& PluginPath)
{
	if (!PluginManager.IsValid()) { return nullptr; }
	auto Plugin = PluginManager->FindPluginFromPath(PluginPath);

	return Plugin ? GLibConversions::ToBlueprintable<UGLibPlugin>(Plugin) : nullptr;
}

void UGLibPluginManager::GetEnabledPlugins(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetEnabledPlugins();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithContent(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithContent();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithVerse(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithVerse();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithContentOrVerse(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithContentOrVerse();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetDiscoveredPlugins(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetDiscoveredPlugins();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

bool UGLibPluginManager::AddPluginSearchPath(const FString& ExtraDiscoveryPath, bool bRefresh)
{
	return PluginManager.IsValid() ? PluginManager->AddPluginSearchPath(ExtraDiscoveryPath, bRefresh) : false;
}

TSet<FString> UGLibPluginManager::GetAdditionalPluginSearchPaths()
{
	return PluginManager.IsValid() ? PluginManager->GetAdditionalPluginSearchPaths() : TSet<FString>();
}

void UGLibPluginManager::GetPluginsWithPakFile(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!PluginManager.IsValid()) { return; }
	const auto Plugins = PluginManager->GetPluginsWithPakFile();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::MountNewlyCreatedPlugin(const FString& PluginName)
{
	return PluginManager.IsValid() ? PluginManager->MountNewlyCreatedPlugin(PluginName) : void();
}

void UGLibPluginManager::MountExplicitlyLoadedPlugin(const FString& PluginName)
{
	return PluginManager.IsValid() ? PluginManager->MountExplicitlyLoadedPlugin(PluginName) : void();
}

bool UGLibPluginManager::UnmountExplicitlyLoadedPlugin(const FString& PluginName, FText& OutReason)
{
	return PluginManager.IsValid() ? PluginManager->UnmountExplicitlyLoadedPlugin(PluginName, &OutReason) : false;
}

FName UGLibPluginManager::PackageNameFromModuleName(FName ModuleName)
{
	return PluginManager.IsValid() ? PluginManager->PackageNameFromModuleName(ModuleName) : FName();
}

bool UGLibPluginManager::RequiresTempTargetForCodePlugin(const FGLibProjectDescriptor& ProjectDescriptor, const FString& Platform, EGLibBuildConfiguration Configuration, EGLibBuildTargetType TargetType, FText& OutReason)
{
	const FProjectDescriptor NativeDescriptor = FProjectDescriptor(ProjectDescriptor);
	return PluginManager.IsValid() ? PluginManager->RequiresTempTargetForCodePlugin(&NativeDescriptor, Platform, GLibConversions::ToNative<EBuildConfiguration>(Configuration), GLibConversions::ToNative<EBuildTargetType>(TargetType), OutReason) : false;
}

UGLibPluginManager* UGLibPluginManager::Get()
{
	if (Instance.IsValid())
	{
		return Instance.Get();
	}

	

	Instance = MakeShareable(NewObject<UGLibPluginManager>());
	return Instance.Get();
}
