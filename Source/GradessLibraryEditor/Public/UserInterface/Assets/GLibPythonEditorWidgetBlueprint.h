// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "GLibPythonEditorWidgetBlueprint.generated.h"

/**
 * @brief Python editor utility widget
 * It reloads and opens (if needed) automatically after Unreal startup, so it helps with broken python blueprint nodes
 */
UCLASS()
class GRADESSLIBRARYEDITOR_API UGLibPythonEditorWidgetBlueprint : public UEditorUtilityWidgetBlueprint
{
	GENERATED_BODY()
};
