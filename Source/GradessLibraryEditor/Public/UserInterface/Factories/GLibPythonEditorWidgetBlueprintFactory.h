// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GLibPythonEditorWidgetBlueprintFactory.generated.h"

/**
 * 
 */
UCLASS()
class GRADESSLIBRARYEDITOR_API UGLibPythonEditorWidgetBlueprintFactory : public UFactory
{
	GENERATED_BODY()

	UGLibPythonEditorWidgetBlueprintFactory(const FObjectInitializer& ObjectInitializer);

	// The type of blueprint that will be created
	UPROPERTY(EditAnywhere, Category = WidgetBlueprintFactory)
	TEnumAsByte<EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = WidgetBlueprintFactory, meta = (AllowAbstract = ""))
	TSubclassOf<class UUserWidget> ParentClass;

	// UFactory interface
	virtual bool ConfigureProperties() override;
	virtual bool ShouldShowInNewMenu() const override { return true; }
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool CanCreateNew() const override;
	
	// End of UFactory interface

	UClass* RootWidgetClass;
};
