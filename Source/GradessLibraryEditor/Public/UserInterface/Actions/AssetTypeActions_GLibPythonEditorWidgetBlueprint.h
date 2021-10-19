// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class GRADESSLIBRARYEDITOR_API FAssetTypeActions_GLibPythonEditorWidgetBlueprint : public FAssetTypeActions_Base
{
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
};
