#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPluginReferenceDescriptorConversionShouldWork, "GLib.Wrappers.Projects.FGLibPluginReferenceDescriptor.Conversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FPluginReferenceDescriptorConversionShouldWork::RunTest(const FString& Parameters)
{
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("GradessLibrary");
	const auto& ReferenceDescriptors = Plugin->GetDescriptor().Plugins;

	for (const auto& Descriptor : ReferenceDescriptors)
	{
		FGLibPluginReferenceDescriptor BlueprintDescriptor = Descriptor;
		FPluginReferenceDescriptor ConvertedDescriptor = BlueprintDescriptor;

		TestTrueExpr(ConvertedDescriptor.Name == Descriptor.Name);
		TestTrueExpr(ConvertedDescriptor.bEnabled == Descriptor.bEnabled);
		TestTrueExpr(ConvertedDescriptor.bOptional == Descriptor.bOptional);
		TestTrueExpr(ConvertedDescriptor.Description == Descriptor.Description);
		TestTrueExpr(ConvertedDescriptor.MarketplaceURL == Descriptor.MarketplaceURL);
		TestTrueExpr(ConvertedDescriptor.PlatformAllowList == Descriptor.PlatformAllowList);
		TestTrueExpr(ConvertedDescriptor.PlatformDenyList == Descriptor.PlatformDenyList);
		TestTrueExpr(ConvertedDescriptor.TargetConfigurationAllowList == Descriptor.TargetConfigurationAllowList);
		TestTrueExpr(ConvertedDescriptor.TargetConfigurationDenyList == Descriptor.TargetConfigurationDenyList);
		TestTrueExpr(ConvertedDescriptor.TargetAllowList == Descriptor.TargetAllowList);
		TestTrueExpr(ConvertedDescriptor.TargetDenyList == Descriptor.TargetDenyList);
		TestTrueExpr(ConvertedDescriptor.SupportedTargetPlatforms == Descriptor.SupportedTargetPlatforms);
		TestTrueExpr(ConvertedDescriptor.bHasExplicitPlatforms == Descriptor.bHasExplicitPlatforms);
	}

	return true;
}
