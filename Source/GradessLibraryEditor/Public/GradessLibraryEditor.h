// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetTypeActions_GLibPythonEditorWidgetBlueprint;

class FGradessLibraryEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FAssetTypeActions_GLibPythonEditorWidgetBlueprint> Actions;

	void RegisterPythonEditorWidgetActions();

	void UnregisterPythonEditorWidgetActions();
};
