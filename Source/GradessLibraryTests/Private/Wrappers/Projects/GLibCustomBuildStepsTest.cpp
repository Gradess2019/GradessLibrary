#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConversionShouldWork, "GLib.Wrappers.Projects.UGLibCustomBuildSteps.Conversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)


bool FConversionShouldWork::RunTest(const FString& Parameters)
{
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("ActorLayerUtilities"));

	UTEST_TRUE("Plugin should be valid", Plugin.IsValid());

	const auto& NativeDescriptor = Plugin->GetDescriptor();
	FGLibCustomBuildSteps BlueprintBuildSteps = NativeDescriptor.PreBuildSteps;
	FCustomBuildSteps NativeBuildSteps = BlueprintBuildSteps;

	UTEST_TRUE("Build steps should be same",  NativeDescriptor.PreBuildSteps.HostPlatformToCommands.OrderIndependentCompareEqual(NativeBuildSteps.HostPlatformToCommands));
	
	return true;
}
