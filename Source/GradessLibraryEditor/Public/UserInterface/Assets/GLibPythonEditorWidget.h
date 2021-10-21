// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "GLibPythonEditorWidget.generated.h"


/**
 * @brief A simple python editor utility widget that registers itself automatically
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	)
)
class GRADESSLIBRARYEDITOR_API UGLibPythonEditorWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
