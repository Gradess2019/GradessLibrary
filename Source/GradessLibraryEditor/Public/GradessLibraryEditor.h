// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FAssetTypeActions_Base;

/**
 * @brief GLib editor module
 */
class FGradessLibraryEditorModule : public IModuleInterface
{
public:
#pragma region IModuleInterface implementation
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
#pragma endregion IModuleInterface implementation

private:
	/**
	 * @brief Asset type actions. Currently used to add python editor widget asset to editor context menu
	 */
	TSharedPtr<FAssetTypeActions_Base> Actions;

	/**
	 * @brief Registers python editor widget asset to editor
	 */
	void RegisterPythonEditorWidgetActions();
	
	/**
	 * @brief Unregister python editor widget asset from editor
	 */
	void UnregisterPythonEditorWidgetActions();
};
