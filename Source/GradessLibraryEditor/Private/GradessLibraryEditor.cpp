// Copyright Epic Games, Inc. All Rights Reserved.

#include "GradessLibraryEditor.h"

#include "AssetToolsModule.h"
#include "UserInterface/Actions/AssetTypeActions_GLibPythonEditorWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "FGradessLibraryEditorModule"

void FGradessLibraryEditorModule::StartupModule()
{
	RegisterPythonEditorWidgetActions();
}

void FGradessLibraryEditorModule::ShutdownModule()
{
	UnregisterPythonEditorWidgetActions();
}

void FGradessLibraryEditorModule::RegisterPythonEditorWidgetActions()
{
	auto& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	Actions = MakeShareable(new FAssetTypeActions_GLibPythonEditorWidgetBlueprint());
	AssetTools.RegisterAssetTypeActions(Actions.ToSharedRef());
}

void FGradessLibraryEditorModule::UnregisterPythonEditorWidgetActions()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		auto& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(Actions.ToSharedRef());
	}

	Actions.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGradessLibraryEditorModule, GradessLibraryEditor)
