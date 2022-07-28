#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Core/GLibPlugin.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnumConversionShouldWork, "GLib.Wrappers.Projects.FGLibLocalizationTargetDescriptor.EnumConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStructConversionShouldWork, "GLib.Wrappers.Projects.FGLibLocalizationTargetDescriptor.StructConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FEnumConversionShouldWork::RunTest(const FString& Parameters)
{
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::Never) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::Never));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::Always) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::Always));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::Editor) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::Editor));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::Game) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::Game));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::PropertyNames) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::PropertyNames));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::ToolTips) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::ToolTips));
	TestTrueExpr(static_cast<int32>(EGLibLocalizationTargetDescriptorLoadingPolicy::Max) == static_cast<int32>(ELocalizationTargetDescriptorLoadingPolicy::Max));

	return true;
}

bool FStructConversionShouldWork::RunTest(const FString& Parameters)
{
    FLocalizationTargetDescriptor Descriptor;;
	Descriptor.Name = "Test";
	Descriptor.LoadingPolicy = ELocalizationTargetDescriptorLoadingPolicy::Never;

	FGLibLocalizationTargetDescriptor GLibDescriptor = Descriptor;

	TestTrue("Names should be equal", GLibDescriptor.Name == Descriptor.Name);
	TestTrue("Loading policy should be equal", static_cast<int32>(GLibDescriptor.LoadingPolicy) == static_cast<int32>(Descriptor.LoadingPolicy));

	return true;
}


