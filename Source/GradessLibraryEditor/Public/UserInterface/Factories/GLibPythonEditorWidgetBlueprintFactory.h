// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "GLibPythonEditorWidgetBlueprintFactory.generated.h"

UCLASS()
class GRADESSLIBRARYEDITOR_API UGLibPythonEditorWidgetBlueprintFactory : public UFactory
{
	GENERATED_BODY()

	UGLibPythonEditorWidgetBlueprintFactory(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief The type of blueprint that will be created
	 */
	UPROPERTY(
		EditAnywhere,
		Category = "GLib|PythonEditorWidgetBlueprintFactory"
	)
	TEnumAsByte<EBlueprintType> BlueprintType;
	
	/**
	 * @brief The parent class of the created blueprint
	 */
	UPROPERTY(
		EditAnywhere,
		Category = "GLib|PythonEditorWidgetBlueprintFactory",
		meta = (
			AllowAbstract = "false"
			)
	)
	TSubclassOf<class UUserWidget> ParentClass;

#pragma region UFactory implementation
	virtual bool ConfigureProperties() override;

	virtual bool ShouldShowInNewMenu() const override { return true; }

	virtual UObject* FactoryCreateNew(
		UClass* Class,
		UObject* InParent,
		FName Name,
		EObjectFlags
		Flags,
		UObject* Context,
		FFeedbackContext* Warn
	) override;
	
	bool IsValidParentClass() const;

	UEditorUtilityWidgetBlueprint* CreateEditorWidget(
		UObject* InParent,
		FName Name
	) const;
	
	virtual bool CanCreateNew() const override;
#pragma endregion UFactory implementation

	UClass* RootWidgetClass;
};
