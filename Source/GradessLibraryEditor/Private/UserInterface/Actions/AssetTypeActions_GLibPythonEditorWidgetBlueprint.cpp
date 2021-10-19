// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Actions/AssetTypeActions_GLibPythonEditorWidgetBlueprint.h"
#include "WidgetBlueprintEditor.h"
#include "UserInterface/Assets/GLibPythonEditorWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "GLibAssetTypeActions"

FText FAssetTypeActions_GLibPythonEditorWidgetBlueprint::GetName() const
{
	return LOCTEXT("AssetTypeActions_GLibPythonEditorWidget", "Python Editor Widget");
}

UClass* FAssetTypeActions_GLibPythonEditorWidgetBlueprint::GetSupportedClass() const
{
	return UGLibPythonEditorWidgetBlueprint::StaticClass();
}

FColor FAssetTypeActions_GLibPythonEditorWidgetBlueprint::GetTypeColor() const
{
	return FColor(0, 169, 255);
}

uint32 FAssetTypeActions_GLibPythonEditorWidgetBlueprint::GetCategories()
{
	if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) { return EAssetTypeCategories::Misc; }

	const auto& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	return AssetTools.FindAdvancedAssetCategory("EditorUtilities");;
}

#undef LOCTEXT_NAMESPACE