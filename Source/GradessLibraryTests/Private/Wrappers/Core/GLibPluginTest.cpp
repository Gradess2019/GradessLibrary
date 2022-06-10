#include "CoreMinimal.h"

#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"

#include "Modules/ModuleManager.h"
#include "Wrappers/Core/GLibGenericPlatformMisc.h"
#include "Wrappers/Core/GLibPlugin.h"
#include "Wrappers/Projects/Interfaces/GLibPluginManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGLibPlugin, "GLib.Wrappers.Core.UGLibPlugin", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::MediumPriority)


bool FGLibPlugin::RunTest(const FString& Parameters)
{
	IPluginManager& PluginManager = IPluginManager::Get();
	const TSharedPtr<IPlugin> Plugin = PluginManager.FindPlugin("GradessLibrary");

	UTEST_VALID("Plugin is not null", Plugin);

	TestNotNull(TEXT("TSharedPtr<IPlugin> wrapper is not null"), UGLibPlugin::CreateWrapper(Plugin));
	TestNotNull(TEXT("TSharedPtr<IPlugin> wrapper is not null"), UGLibPlugin::CreateWrapper(Plugin.ToSharedRef()));

	UGLibPlugin* WrappedPlugin = UGLibPlugin::CreateWrapper(Plugin);
	UTEST_NOT_NULL("WrappedPlugin", WrappedPlugin);

	WrappedPlugin->SetPlugin(Plugin);
	TestNotNull("SetPlugin() TSharedPtr", WrappedPlugin->GetPlugin().Get());

	WrappedPlugin->SetPlugin(Plugin);
	TestNotNull("SetPlugin() TSharedRef", WrappedPlugin->GetPlugin().Get());


	TestEqual("GetName()", WrappedPlugin->GetName(), "GradessLibrary");
	TestEqual("GetFriendlyName()", WrappedPlugin->GetFriendlyName(), "GradessLibrary");
	TestEqual("GetDescriptorFileName()", WrappedPlugin->GetDescriptorFileName(), FPaths::ProjectPluginsDir() / "GradessLibrary" / "GradessLibrary.uplugin");
	TestEqual("GetBaseDir()", WrappedPlugin->GetBaseDir(), FPaths::ProjectPluginsDir() / "GradessLibrary");
	TestEqual("GetContentDir()", WrappedPlugin->GetContentDir(), FPaths::ProjectPluginsDir() / "GradessLibrary" / "Content");
	TestEqual("GetMountedAssetPath()", WrappedPlugin->GetMountedAssetPath(), "/GradessLibrary/");
	TestEqual("GetLoadedFrom()", WrappedPlugin->GetLoadedFrom(), EGLibPluginLoadedFrom::Project);
	TestTrue("IsValid()", WrappedPlugin->IsValid());
	TestTrue("IsEnabled()", WrappedPlugin->IsEnabled());
	TestTrue("IsEnabledByDefault()", WrappedPlugin->IsEnabledByDefault(false));
	TestTrue("CanContainContent()", WrappedPlugin->CanContainContent());
	TestFalse("IsHidden()", WrappedPlugin->IsHidden());
	TestFalse("CanContainVerse()", WrappedPlugin->CanContainVerse());

	
	const auto Descriptor = WrappedPlugin->GetDescriptor();
	TestEqual("Descriptor.FriendlyName", Descriptor.FriendlyName, "GradessLibrary");


	// TODO GLIB-15
	// const auto EmptyDescriptor = FGLibPluginDescriptor();
	// FText FailReason;
	//
	// TestTrue("UpdateDescriptor()", WrappedPlugin->UpdateDescriptor(EmptyDescriptor, FailReason));
	// TestEqual("EmptyDescriptor == WrappedPlugin->GetDescriptor()", EmptyDescriptor.FriendlyName, WrappedPlugin->GetDescriptor().FriendlyName);
	//
	//
	// WrappedPlugin->UpdateDescriptor(Descriptor, FailReason);
	
	return true;
}

