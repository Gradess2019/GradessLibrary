#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

#include "Interfaces/IProjectManager.h"

#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectDescriptorVersionConversionShouldWork, "GLib.Wrappers.Projects.FGLibProjectDescriptor.ProjectDescriptorVersionConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectDescriptorConversionShouldWork, "GLib.Wrappers.Projects.FGLibProjectDescriptor.ProjectDescriptorConversion", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)

bool FProjectDescriptorVersionConversionShouldWork::RunTest(const FString& Parameters)
{
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::Invalid) == static_cast<int32>(EProjectDescriptorVersion::Invalid));
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::Initial) == static_cast<int32>(EProjectDescriptorVersion::Initial));
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::NameHash) == static_cast<int32>(EProjectDescriptorVersion::NameHash));
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::ProjectPluginUnification) == static_cast<int32>(EProjectDescriptorVersion::ProjectPluginUnification));
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::LatestPlusOne) == static_cast<int32>(EProjectDescriptorVersion::LatestPlusOne));
	TestTrueExpr(static_cast<int32>(EGLibProjectDescriptorVersion::Latest) == static_cast<int32>(EProjectDescriptorVersion::Latest));
	
	return true;
}

bool FProjectDescriptorConversionShouldWork::RunTest(const FString& Parameters)
{
	const auto ProjectDescriptor = IProjectManager::Get().GetCurrentProject();

	FGLibProjectDescriptor BlueprintProject = *ProjectDescriptor;
	FProjectDescriptor ConvertedDescriptor = BlueprintProject;

	TestTrueExpr(ConvertedDescriptor.FileVersion == ProjectDescriptor->FileVersion);
	TestTrueExpr(ConvertedDescriptor.EngineAssociation == ProjectDescriptor->EngineAssociation);
	TestTrueExpr(ConvertedDescriptor.Category == ProjectDescriptor->Category);
	TestTrueExpr(ConvertedDescriptor.Description == ProjectDescriptor->Description);
	TestTrueExpr(ConvertedDescriptor.TargetPlatforms == ProjectDescriptor->TargetPlatforms);
	TestTrueExpr(ConvertedDescriptor.EpicSampleNameHash == ProjectDescriptor->EpicSampleNameHash);
	TestTrueExpr(ConvertedDescriptor.PreBuildSteps.HostPlatformToCommands.OrderIndependentCompareEqual(ProjectDescriptor->PreBuildSteps.HostPlatformToCommands));
	TestTrueExpr(ConvertedDescriptor.PostBuildSteps.HostPlatformToCommands.OrderIndependentCompareEqual(ProjectDescriptor->PostBuildSteps.HostPlatformToCommands));
	TestTrueExpr(ConvertedDescriptor.bIsEnterpriseProject == ProjectDescriptor->bIsEnterpriseProject);

	// TODO: GLIB-17
	// TestTrueExpr(ConvertedDescriptor.Modules == ProjectDescriptor.Modules);
	// TestTrueExpr(ConvertedDescriptor.Plugins == ProjectDescriptor.Plugins);
	return true;
}
