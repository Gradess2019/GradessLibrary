#include "CoreMinimal.h"

#include "Interfaces/IPluginManager.h"

#include "Misc/AutomationTest.h"

#include "Modules/ModuleManager.h"

#include "Wrappers/Core/Modules/GLibModuleManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGLibModuleLoadResultTest, "GLib.Wrappers.Core.Modules.GLibModuleManager.EGLibModuleLoadResult", EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGLibPluginLoadedFrom, "GLib.Wrappers.Core.Modules.GLibModuleManager.EGLibPluginLoadedFrom", EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority)


bool FGLibModuleLoadResultTest::RunTest(const FString& Parameters)
{
	TestEqual("EGLibModuleLoadResult::Success == EModuleLoadResult::Success", static_cast<int32>(EGLibModuleLoadResult::Success), static_cast<int32>(EModuleLoadResult::Success));
	TestEqual("EGLibModuleLoadResult::FileNotFound == EModuleLoadResult::FileNotFound", static_cast<int32>(EGLibModuleLoadResult::FileNotFound), static_cast<int32>(EModuleLoadResult::FileNotFound));
	TestEqual("EGLibModuleLoadResult::FileIncompatible == EModuleLoadResult::FileIncompatible", static_cast<int32>(EGLibModuleLoadResult::FileIncompatible), static_cast<int32>(EModuleLoadResult::FileIncompatible));
	TestEqual("EGLibModuleLoadResult::CouldNotBeLoadedByOS == EModuleLoadResult::CouldNotBeLoadedByOS", static_cast<int32>(EGLibModuleLoadResult::CouldNotBeLoadedByOS), static_cast<int32>(EModuleLoadResult::CouldNotBeLoadedByOS));
	TestEqual("EGLibModuleLoadResult::FailedToInitialize == EModuleLoadResult::FailedToInitialize", static_cast<int32>(EGLibModuleLoadResult::FailedToInitialize), static_cast<int32>(EModuleLoadResult::FailedToInitialize));

	return true;
}

bool FGLibPluginLoadedFrom::RunTest(const FString& Parameters)
{
	TestEqual("EGLibPluginLoadedFrom::Engine == EPluginLoadedFrom::Engine", static_cast<int32>(EGLibPluginLoadedFrom::Engine), static_cast<int32>(EPluginLoadedFrom::Engine));
	TestEqual("EGLibPluginLoadedFrom::Project == EPluginLoadedFrom::Project", static_cast<int32>(EGLibPluginLoadedFrom::Project), static_cast<int32>(EPluginLoadedFrom::Project));

	return true;
}
