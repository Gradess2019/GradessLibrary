#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHostTypeConversionShouldWork, "GLib.Wrappers.Projects.FGLibModuleDescriptor.HostTypeConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadingPhaseConversionShouldWork, "GLib.Wrappers.Projects.FGLibModuleDescriptor.LoadingPhaseConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModuleDescriptorConversionShouldWork, "GLib.Wrappers.Projects.FGLibModuleDescriptor.ModuleDescriptorConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FHostTypeConversionShouldWork::RunTest(const FString& Parameters)
{
	TestTrueExpr(static_cast<int32>(EGLibHostType::Runtime) == static_cast<int32>(EHostType::Runtime));
	TestTrueExpr(static_cast<int32>(EGLibHostType::RuntimeNoCommandlet) == static_cast<int32>(EHostType::RuntimeNoCommandlet));
	TestTrueExpr(static_cast<int32>(EGLibHostType::RuntimeAndProgram) == static_cast<int32>(EHostType::RuntimeAndProgram));
	TestTrueExpr(static_cast<int32>(EGLibHostType::CookedOnly) == static_cast<int32>(EHostType::CookedOnly));
	TestTrueExpr(static_cast<int32>(EGLibHostType::UncookedOnly) == static_cast<int32>(EHostType::UncookedOnly));
	TestTrueExpr(static_cast<int32>(EGLibHostType::Developer) == static_cast<int32>(EHostType::Developer));
	TestTrueExpr(static_cast<int32>(EGLibHostType::DeveloperTool) == static_cast<int32>(EHostType::DeveloperTool));
	TestTrueExpr(static_cast<int32>(EGLibHostType::Editor) == static_cast<int32>(EHostType::Editor));
	TestTrueExpr(static_cast<int32>(EGLibHostType::EditorNoCommandlet) == static_cast<int32>(EHostType::EditorNoCommandlet));
	TestTrueExpr(static_cast<int32>(EGLibHostType::EditorAndProgram) == static_cast<int32>(EHostType::EditorAndProgram));
	TestTrueExpr(static_cast<int32>(EGLibHostType::Program) == static_cast<int32>(EHostType::Program));
	TestTrueExpr(static_cast<int32>(EGLibHostType::ServerOnly) == static_cast<int32>(EHostType::ServerOnly));
	TestTrueExpr(static_cast<int32>(EGLibHostType::ClientOnly) == static_cast<int32>(EHostType::ClientOnly));
	TestTrueExpr(static_cast<int32>(EGLibHostType::ClientOnlyNoCommandlet) == static_cast<int32>(EHostType::ClientOnlyNoCommandlet));
	TestTrueExpr(static_cast<int32>(EGLibHostType::Max) == static_cast<int32>(EHostType::Max));

	return true;
}

bool FLoadingPhaseConversionShouldWork::RunTest(const FString& Parameters)
{
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::EarliestPossible) == static_cast<int32>(ELoadingPhase::EarliestPossible));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PostConfigInit) == static_cast<int32>(ELoadingPhase::PostConfigInit));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PostSplashScreen) == static_cast<int32>(ELoadingPhase::PostSplashScreen));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PreEarlyLoadingScreen) == static_cast<int32>(ELoadingPhase::PreEarlyLoadingScreen));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PreLoadingScreen) == static_cast<int32>(ELoadingPhase::PreLoadingScreen));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PreDefault) == static_cast<int32>(ELoadingPhase::PreDefault));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::Default) == static_cast<int32>(ELoadingPhase::Default));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PostDefault) == static_cast<int32>(ELoadingPhase::PostDefault));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::PostEngineInit) == static_cast<int32>(ELoadingPhase::PostEngineInit));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::None) == static_cast<int32>(ELoadingPhase::None));
	TestTrueExpr(static_cast<int32>(EGLibLoadingPhase::Max) == static_cast<int32>(ELoadingPhase::Max));

	return true;
}

bool FModuleDescriptorConversionShouldWork::RunTest(const FString& Parameters)
{
    const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("GradessLibrary");
	const auto ModuleDescriptors = Plugin->GetDescriptor().Modules;

    for (const auto& NativeDescriptor : ModuleDescriptors)
    {
    	FGLibModuleDescriptor Descriptor = NativeDescriptor;
    	FModuleDescriptor NativeDescriptorFromBlueprint = Descriptor;

    	TestTrueExpr(NativeDescriptorFromBlueprint.Name == NativeDescriptor.Name);
    	TestTrueExpr(NativeDescriptorFromBlueprint.Type == NativeDescriptor.Type);
    	TestTrueExpr(NativeDescriptorFromBlueprint.LoadingPhase == NativeDescriptor.LoadingPhase);
    	TestTrueExpr(NativeDescriptorFromBlueprint.PlatformAllowList == NativeDescriptor.PlatformAllowList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.PlatformDenyList == NativeDescriptor.PlatformDenyList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.TargetAllowList == NativeDescriptor.TargetAllowList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.TargetDenyList == NativeDescriptor.TargetDenyList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.TargetConfigurationAllowList == NativeDescriptor.TargetConfigurationAllowList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.TargetConfigurationDenyList == NativeDescriptor.TargetConfigurationDenyList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.ProgramAllowList == NativeDescriptor.ProgramAllowList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.ProgramDenyList == NativeDescriptor.ProgramDenyList);
    	TestTrueExpr(NativeDescriptorFromBlueprint.AdditionalDependencies == NativeDescriptor.AdditionalDependencies);
    	TestTrueExpr(NativeDescriptorFromBlueprint.bHasExplicitPlatforms == NativeDescriptor.bHasExplicitPlatforms);
    }

	return true;
}


