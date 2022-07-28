// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/CoreDelegates.h"
#include "Wrappers/GLibConversions.h"
#include "Wrappers/Core/GLibPlugin.h"

UGLibPluginManager::UGLibPluginManager()
{
	PluginManager = &IPluginManager::Get();
	if (!PluginManager)
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
		OnNewPluginCreated.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(TSharedPtr<IPlugin>(MakeShareable<IPlugin>(&PluginInfo))));
	});
	
	PluginManager->OnNewPluginMounted().AddLambda([this](IPlugin& PluginInfo)
	{
		OnNewPluginMounted.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(TSharedPtr<IPlugin>(MakeShareable<IPlugin>(&PluginInfo))));
	});
	
	PluginManager->OnPluginEdited().AddLambda([this](IPlugin& PluginInfo)
	{
		OnPluginEdited.Broadcast(GLibConversions::ToBlueprintable<UGLibPlugin>(TSharedPtr<IPlugin>(MakeShareable<IPlugin>(&PluginInfo))));
	});

	FCoreDelegates::OnEnginePreExit.AddLambda([this]()
	{
		RemoveFromRoot();
	});
}

void UGLibPluginManager::RefreshPluginsList()
{
	if (!IsValidManager()) { return; }
	PluginManager->RefreshPluginsList();
}

bool UGLibPluginManager::AddToPluginsList(const FString& PluginFilename, FText& OutFailReason)
{
	return IsValidManager() ? PluginManager->AddToPluginsList(PluginFilename, &OutFailReason) : false;
}

bool UGLibPluginManager::LoadModulesForEnabledPlugins(const EGLibLoadingPhase LoadingPhase)
{
	return IsValidManager() ? PluginManager->LoadModulesForEnabledPlugins(GLibConversions::ToNative<ELoadingPhase::Type>(LoadingPhase)) : false;
}

EGLibLoadingPhase UGLibPluginManager::GetLastCompletedLoadingPhase()
{
	return IsValidManager() ? GLibConversions::ToBlueprintable<EGLibLoadingPhase>(PluginManager->GetLastCompletedLoadingPhase()) : EGLibLoadingPhase::None;
}

void UGLibPluginManager::GetLocalizationPathsForEnabledPlugins(TArray<FString>& OutLocResPaths)
{
	if (!IsValidManager()) { return; }
	PluginManager->GetLocalizationPathsForEnabledPlugins(OutLocResPaths);
}

bool UGLibPluginManager::AreRequiredPluginsAvailable()
{
	return IsValidManager() ? PluginManager->AreRequiredPluginsAvailable() : false;
}

bool UGLibPluginManager::CheckModuleCompatibility(TArray<FString>& OutIncompatibleModules, TArray<FString>& OutIncompatibleEngineModules)
{
	return IsValidManager() ? PluginManager->CheckModuleCompatibility(OutIncompatibleModules, OutIncompatibleEngineModules) : false;
}

UGLibPlugin* UGLibPluginManager::FindPlugin(const FString& Name)
{
	return IsValidManager() ? GLibConversions::ToBlueprintable<UGLibPlugin>(PluginManager->FindPlugin(*Name)) : nullptr;
}

UGLibPlugin* UGLibPluginManager::FindPluginFromPath(const FString& PluginPath)
{
	if (!IsValidManager()) { return nullptr; }
	auto Plugin = PluginManager->FindPluginFromPath(PluginPath);

	return Plugin ? GLibConversions::ToBlueprintable<UGLibPlugin>(Plugin) : nullptr;
}

void UGLibPluginManager::GetEnabledPlugins(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetEnabledPlugins();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithContent(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithContent();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithVerse(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithVerse();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetEnabledPluginsWithContentOrVerse(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetEnabledPluginsWithContentOrVerse();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::GetDiscoveredPlugins(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetDiscoveredPlugins();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

bool UGLibPluginManager::AddPluginSearchPath(const FString& ExtraDiscoveryPath, bool bRefresh)
{
	return IsValidManager() ? PluginManager->AddPluginSearchPath(ExtraDiscoveryPath, bRefresh) : false;
}

TSet<FString> UGLibPluginManager::GetAdditionalPluginSearchPaths()
{
	return IsValidManager() ? PluginManager->GetAdditionalPluginSearchPaths() : TSet<FString>();
}

void UGLibPluginManager::GetPluginsWithPakFile(TArray<UGLibPlugin*>& OutPlugins)
{
	if (!IsValidManager()) { return; }
	const auto Plugins = PluginManager->GetPluginsWithPakFile();
	GLibConversions::ToBlueprintable<UGLibPlugin>(Plugins, OutPlugins);
}

void UGLibPluginManager::MountNewlyCreatedPlugin(const FString& PluginName)
{
	return IsValidManager() ? PluginManager->MountNewlyCreatedPlugin(PluginName) : void();
}

void UGLibPluginManager::MountExplicitlyLoadedPlugin(const FString& PluginName)
{
	return IsValidManager() ? PluginManager->MountExplicitlyLoadedPlugin(PluginName) : void();
}

bool UGLibPluginManager::UnmountExplicitlyLoadedPlugin(const FString& PluginName, FText& OutReason)
{
	return IsValidManager() ? PluginManager->UnmountExplicitlyLoadedPlugin(PluginName, &OutReason) : false;
}

FName UGLibPluginManager::PackageNameFromModuleName(FName ModuleName)
{
	return IsValidManager() ? PluginManager->PackageNameFromModuleName(ModuleName) : FName();
}

bool UGLibPluginManager::RequiresTempTargetForCodePlugin(const FGLibProjectDescriptor& ProjectDescriptor, const FString& Platform, EGLibBuildConfiguration Configuration, EGLibBuildTargetType TargetType, FText& OutReason)
{
	const FProjectDescriptor NativeDescriptor = FProjectDescriptor(ProjectDescriptor);
	return IsValidManager() ? PluginManager->RequiresTempTargetForCodePlugin(&NativeDescriptor, Platform, GLibConversions::ToNative<EBuildConfiguration>(Configuration), GLibConversions::ToNative<EBuildTargetType>(TargetType), OutReason) : false;
}

bool UGLibPluginManager::IsValidManager()
{
	return PluginManager != nullptr;
}

UGLibPluginManager* UGLibPluginManager::Get()
{
	return GetMutableDefault<UGLibPluginManager>();
}
