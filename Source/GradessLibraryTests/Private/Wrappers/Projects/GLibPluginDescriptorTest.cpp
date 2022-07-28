#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Projects/GLibPluginDescriptor.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginEnabledByDefaultConversionShouldWork, "GLib.Wrappers.Projects.FGLibPluginDescriptor.PluginEnabledByDefaultConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginDescriptorConversionShouldWork, "GLib.Wrappers.Projects.FGLibPluginDescriptor.PluginDescriptorConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FPluginEnabledByDefaultConversionShouldWork::RunTest(const FString& Parameters)
{
	TestTrueExpr(static_cast<int32>(EGLibPluginEnabledByDefault::Unspecified) == static_cast<int32>(EPluginEnabledByDefault::Unspecified));
	TestTrueExpr(static_cast<int32>(EGLibPluginEnabledByDefault::Enabled) == static_cast<int32>(EPluginEnabledByDefault::Enabled));
	TestTrueExpr(static_cast<int32>(EGLibPluginEnabledByDefault::Disabled) == static_cast<int32>(EPluginEnabledByDefault::Disabled));

	return true;
}

bool FPluginDescriptorConversionShouldWork::RunTest(const FString& Parameters)
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("GradessLibrary");

	auto& Descriptor = Plugin->GetDescriptor();

	FGLibPluginDescriptor BlueprintDescriptor = Descriptor;
	FPluginDescriptor NativeDescriptorFromBlueprint = BlueprintDescriptor;

	TestTrueExpr(NativeDescriptorFromBlueprint.Version == Descriptor.Version);
	TestTrueExpr(NativeDescriptorFromBlueprint.VersionName == Descriptor.VersionName);
	TestTrueExpr(NativeDescriptorFromBlueprint.FriendlyName == Descriptor.FriendlyName);
	TestTrueExpr(NativeDescriptorFromBlueprint.Description == Descriptor.Description);
	TestTrueExpr(NativeDescriptorFromBlueprint.Category == Descriptor.Category);
	TestTrueExpr(NativeDescriptorFromBlueprint.CreatedBy == Descriptor.CreatedBy);
	TestTrueExpr(NativeDescriptorFromBlueprint.CreatedByURL == Descriptor.CreatedByURL);
	TestTrueExpr(NativeDescriptorFromBlueprint.DocsURL == Descriptor.DocsURL);
	TestTrueExpr(NativeDescriptorFromBlueprint.MarketplaceURL == Descriptor.MarketplaceURL);
	TestTrueExpr(NativeDescriptorFromBlueprint.SupportURL == Descriptor.SupportURL);
	TestTrueExpr(NativeDescriptorFromBlueprint.EngineVersion == Descriptor.EngineVersion);
	TestTrueExpr(NativeDescriptorFromBlueprint.EditorCustomVirtualPath == Descriptor.EditorCustomVirtualPath);
	TestTrueExpr(NativeDescriptorFromBlueprint.SupportedTargetPlatforms == Descriptor.SupportedTargetPlatforms);
	TestTrueExpr(NativeDescriptorFromBlueprint.SupportedPrograms == Descriptor.SupportedPrograms);
	TestTrueExpr(NativeDescriptorFromBlueprint.ParentPluginName == Descriptor.ParentPluginName);
	TestTrueExpr(NativeDescriptorFromBlueprint.EnabledByDefault == Descriptor.EnabledByDefault);
	TestTrueExpr(NativeDescriptorFromBlueprint.bCanContainContent == Descriptor.bCanContainContent);
	TestTrueExpr(NativeDescriptorFromBlueprint.bCanContainVerse == Descriptor.bCanContainVerse);
	TestTrueExpr(NativeDescriptorFromBlueprint.bIsBetaVersion == Descriptor.bIsBetaVersion);
	TestTrueExpr(NativeDescriptorFromBlueprint.bIsExperimentalVersion == Descriptor.bIsExperimentalVersion);
	TestTrueExpr(NativeDescriptorFromBlueprint.bInstalled == Descriptor.bInstalled);
	TestTrueExpr(NativeDescriptorFromBlueprint.bRequiresBuildPlatform == Descriptor.bRequiresBuildPlatform);
	TestTrueExpr(NativeDescriptorFromBlueprint.bIsHidden == Descriptor.bIsHidden);
	TestTrueExpr(NativeDescriptorFromBlueprint.bExplicitlyLoaded == Descriptor.bExplicitlyLoaded);
	TestTrueExpr(NativeDescriptorFromBlueprint.bHasExplicitPlatforms == Descriptor.bHasExplicitPlatforms);
	TestTrueExpr(NativeDescriptorFromBlueprint.bIsPluginExtension == Descriptor.bIsPluginExtension);
	
	TestTrueExpr(NativeDescriptorFromBlueprint.PreBuildSteps.HostPlatformToCommands.OrderIndependentCompareEqual(Descriptor.PreBuildSteps.HostPlatformToCommands));
	TestTrueExpr(NativeDescriptorFromBlueprint.PostBuildSteps.HostPlatformToCommands.OrderIndependentCompareEqual(Descriptor.PostBuildSteps.HostPlatformToCommands));

	// TODO: GLib-17
	// TestTrueExpr(NativeDescriptorFromBlueprint.Modules == Descriptor.Modules);
	// TestTrueExpr(NativeDescriptorFromBlueprint.LocalizationTargets == Descriptor.LocalizationTargets);
	// TestTrueExpr(NativeDescriptorFromBlueprint.Plugins == Descriptor.Plugins);

	return true;
}
