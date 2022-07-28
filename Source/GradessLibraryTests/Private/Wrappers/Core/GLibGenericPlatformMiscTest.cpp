#include "CoreMinimal.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Core/GLibGenericPlatformMisc.h"
#include "Wrappers/Core/Modules/GLibModuleManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGLibBuildTargetType, "GLib.Wrappers.Core.GLibGenericPlatformMisc.EGLibBuildTargetType", EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGLibBuildConfiguration, "GLib.Wrappers.Core.GLibGenericPlatformMisc.EGLibBuildConfiguration", EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::HighPriority)


bool FGLibBuildTargetType::RunTest(const FString& Parameters)
{
	TestEqual("EGLibBuildTargetType::Unknown == EBuildTargetType::Unknown", static_cast<int32>(EGLibBuildTargetType::Unknown), static_cast<int32>(EBuildTargetType::Unknown));
	TestEqual("EGLibBuildTargetType::Game == EBuildTargetType::Game", static_cast<int32>(EGLibBuildTargetType::Game), static_cast<int32>(EBuildTargetType::Game));
	TestEqual("EGLibBuildTargetType::Server == EBuildTargetType::Server", static_cast<int32>(EGLibBuildTargetType::Server), static_cast<int32>(EBuildTargetType::Server));
	TestEqual("EGLibBuildTargetType::Client == EBuildTargetType::Client", static_cast<int32>(EGLibBuildTargetType::Client), static_cast<int32>(EBuildTargetType::Client));
	TestEqual("EGLibBuildTargetType::Editor == EBuildTargetType::Editor", static_cast<int32>(EGLibBuildTargetType::Editor), static_cast<int32>(EBuildTargetType::Editor));
	TestEqual("EGLibBuildTargetType::Program == EBuildTargetType::Program", static_cast<int32>(EGLibBuildTargetType::Program), static_cast<int32>(EBuildTargetType::Program));

	return true;
}

bool FGLibBuildConfiguration::RunTest(const FString& Parameters)
{
	TestEqual("EGLibBuildConfiguration::Unknown == EBuildConfiguration::Unknown", static_cast<int32>(EGLibBuildConfiguration::Unknown), static_cast<int32>(EBuildConfiguration::Unknown));
	TestEqual("EGLibBuildConfiguration::Debug == EBuildConfiguration::Debug", static_cast<int32>(EGLibBuildConfiguration::Debug), static_cast<int32>(EBuildConfiguration::Debug));
	TestEqual("EGLibBuildConfiguration::DebugGame == EBuildConfiguration::DebugGame", static_cast<int32>(EGLibBuildConfiguration::DebugGame), static_cast<int32>(EBuildConfiguration::DebugGame));
	TestEqual("EGLibBuildConfiguration::Development == EBuildConfiguration::Development", static_cast<int32>(EGLibBuildConfiguration::Development), static_cast<int32>(EBuildConfiguration::Development));
	TestEqual("EGLibBuildConfiguration::Shipping == EBuildConfiguration::Shipping", static_cast<int32>(EGLibBuildConfiguration::Shipping), static_cast<int32>(EBuildConfiguration::Shipping));
	TestEqual("EGLibBuildConfiguration::Test == EBuildConfiguration::Test", static_cast<int32>(EGLibBuildConfiguration::Test), static_cast<int32>(EBuildConfiguration::Test));

	return true;
}
