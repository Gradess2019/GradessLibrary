#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Core/GLibPlugin.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginShouldBeAdded, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.AddToPluginsList", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRequiredPluginsShouldBeAvailable, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.AreRequiredPluginsAvailable", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModuleShouldBeCompatible, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.CheckModuleCompatibility", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginShouldBeFound, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.FindPlugin", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginsListShouldBeFilled, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.GetEnabledPlugins", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewPluginSearchPathShouldBeAdded, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.AddPluginSearchPath", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginShouldBeWithPakFile, "GLib.Wrappers.Projects.Interfaces.UGLibPluginManager.GetPluginsWithPakFile", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FPluginShouldBeAdded::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();
	const auto PluginFilename = FString(TEXT("GradessLibrary"));

	FText OutFailReason;
	UTEST_TRUE("Add plugin", PluginManager->AddToPluginsList(PluginFilename, OutFailReason));

	UTEST_NOT_NULL("Find plugin", PluginManager->FindPlugin(PluginFilename));
	
	return true;
}

bool FRequiredPluginsShouldBeAvailable::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();

	UTEST_TRUE("Check required plugin availability", PluginManager->AreRequiredPluginsAvailable());
	
	return true;
}

bool FModuleShouldBeCompatible::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();

	TArray<FString> IncompatibleModules;
	TArray<FString> IncompatibleEngineModules;
	UTEST_TRUE("Check required plugin availability", PluginManager->CheckModuleCompatibility(IncompatibleModules, IncompatibleEngineModules));
	UTEST_TRUE("IncompatibleModules is empty", IncompatibleModules.Num() == 0);
	UTEST_TRUE("IncompatibleEngineModules is empty", IncompatibleEngineModules.Num() == 0);
	
	return true;
}

bool FPluginShouldBeFound::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();

	const auto PluginName = FString(TEXT("GradessLibrary"));
	const auto PluginPath = FString(TEXT("/GradessLibrary/MyTestFolder/MyTestAsset.uasset"));
	
	UTEST_TRUE("FindPlugin", IsValid(PluginManager->FindPlugin(PluginName)));
	UTEST_TRUE("FindPluginFromPath", IsValid(PluginManager->FindPluginFromPath(PluginPath)));
	
	return true;
}

bool FPluginsListShouldBeFilled::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();

	TArray<UGLibPlugin*> Plugins;
	PluginManager->GetEnabledPlugins(Plugins);
	UTEST_TRUE("Enabled plugins is not empty", !Plugins.IsEmpty());

	PluginManager->GetEnabledPluginsWithContent(Plugins);
	UTEST_TRUE("Enabled plugins with content array is not empty", !Plugins.IsEmpty());
	
	PluginManager->GetEnabledPluginsWithContentOrVerse(Plugins);
	UTEST_TRUE("Enabled plugins with content or verse array is not empty", !Plugins.IsEmpty());
	
	PluginManager->GetDiscoveredPlugins(Plugins);
	UTEST_TRUE("Deiscovered plugins array is not empty", !Plugins.IsEmpty());

	return true;
}

bool FNewPluginSearchPathShouldBeAdded::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();

	const auto SearchPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("PluginTestPath")));
	const auto bRefresh = false;
	const auto bAdded = PluginManager->AddPluginSearchPath(SearchPath, bRefresh);
	
	UTEST_TRUE("GetAdditionalPluginSearchPaths", bAdded || PluginManager->GetAdditionalPluginSearchPaths().Contains(SearchPath));
	
	return true;
}

bool FPluginShouldBeWithPakFile::RunTest(const FString& Parameters)
{
	const auto PluginManager = UGLibPluginManager::Get();
	const auto PluginName = FString(TEXT("GradessLibrary"));
	
	TArray<UGLibPlugin*> Plugins;
	PluginManager->GetPluginsWithPakFile(Plugins);
	
	const auto Element = Plugins.FindByPredicate([PluginName](UGLibPlugin* Element)
	{
		return Element->GetName() == PluginName;
	});
	
	UTEST_TRUE("Plugin with pak is invalid", !Element || !IsValid(*Element));

	return true;
}

