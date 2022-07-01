// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Factories/GLibPythonEditorWidgetBlueprintFactory.h"

#include "ClassViewerFilter.h"
#include "UMGEditorProjectSettings.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/SClassPickerDialog.h"
#include "UserInterface/Assets/GLibPythonEditorWidget.h"
#include "UserInterface/Assets/GLibPythonEditorWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "UGLibPythonEditorWidgetBlueprintFactory"

class GRADESSLIBRARYEDITOR_API FEditorUtilityWidgetBlueprintFactoryFilter : public IClassViewerFilter
{
public:
	
	/**
	 * @brief All children of these classes will be included unless filtered out by another setting.
	 */
	TSet<const UClass*> AllowedChildrenOfClasses;
	
	/**
	 * @brief Disallowed class flags.
	 */
	EClassFlags DisallowedClassFlags;

	bool IsClassAllowed(
		const FClassViewerInitializationOptions& InInitOptions,
		const UClass* InClass,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs
	) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(
		const FClassViewerInitializationOptions& InInitOptions,
		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs
	) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) !=
			EFilterReturn::Failed;
	}
};

UGLibPythonEditorWidgetBlueprintFactory::UGLibPythonEditorWidgetBlueprintFactory(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UGLibPythonEditorWidgetBlueprint::StaticClass();
	ParentClass = UGLibPythonEditorWidget::StaticClass();
}

bool UGLibPythonEditorWidgetBlueprintFactory::ConfigureProperties()
{
	if (GetDefault<UUMGEditorProjectSettings>()->bUseWidgetTemplateSelector)
	{
		// Load the classviewer module to display a class picker
		FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");

		// Fill in options
		FClassViewerInitializationOptions Options;
		Options.Mode = EClassViewerMode::ClassPicker;
		Options.bShowNoneOption = true;

		Options.ExtraPickerCommonClasses.Add(UHorizontalBox::StaticClass());
		Options.ExtraPickerCommonClasses.Add(UVerticalBox::StaticClass());
		Options.ExtraPickerCommonClasses.Add(UGridPanel::StaticClass());
		Options.ExtraPickerCommonClasses.Add(UCanvasPanel::StaticClass());

		const auto Filter = MakeShared<FEditorUtilityWidgetBlueprintFactoryFilter>();
		Options.ClassFilters.Add(Filter);

		Filter->DisallowedClassFlags = CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists;
		Filter->AllowedChildrenOfClasses.Add(UPanelWidget::StaticClass());

		const FText TitleText = LOCTEXT(
			"CreateWidgetBlueprint",
			"Pick Root Widget for New Python Editor Utility Widget"
		);
		
		return SClassPickerDialog::PickClass(TitleText, Options, RootWidgetClass, UPanelWidget::StaticClass());
	}
	return true;
}

UEditorUtilityWidgetBlueprint* UGLibPythonEditorWidgetBlueprintFactory::CreateEditorWidget(UObject* InParent, FName Name) const
{
	const auto CreatedBlueprint = FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		InParent,
		Name,
		BlueprintType,
		UEditorUtilityWidgetBlueprint::StaticClass(),
		UWidgetBlueprintGeneratedClass::StaticClass(),
		NAME_None
	);

	return CastChecked<UEditorUtilityWidgetBlueprint>(CreatedBlueprint);
}

UObject* UGLibPythonEditorWidgetBlueprintFactory::FactoryCreateNew(
	UClass* Class,
	UObject* InParent,
	FName Name,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	// Make sure we are trying to factory a blueprint, then create and init one
	check(Class->IsChildOf(UGLibPythonEditorWidgetBlueprint::StaticClass()));

	if (!IsValidParentClass()) { return nullptr; };

	// If the root widget selection dialog is not enabled, use a canvas panel as the root by default
	if (!GetDefault<UUMGEditorProjectSettings>()->bUseWidgetTemplateSelector)
	{
		RootWidgetClass = UCanvasPanel::StaticClass();
	}

	UEditorUtilityWidgetBlueprint* CreatedEditorWidget = CreateEditorWidget(InParent, Name);
	
	// Create the selected root widget
	if (CreatedEditorWidget->WidgetTree->RootWidget == nullptr)
	{
		if (TSubclassOf<UPanelWidget> RootWidgetPanel = RootWidgetClass)
		{
			UWidget* Root = CreatedEditorWidget->WidgetTree->ConstructWidget<UWidget>(RootWidgetPanel);
			CreatedEditorWidget->WidgetTree->RootWidget = Root;
		}
	}

	return CreatedEditorWidget;
}

bool UGLibPythonEditorWidgetBlueprintFactory::IsValidParentClass() const
{
	if (ParentClass == nullptr || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass))
	{
		FFormatNamedArguments Args;
		const auto ClassName = ParentClass != nullptr ? FText::FromString(ParentClass->GetName()) : NSLOCTEXT("UnrealEd", "Null", "(null)");
		Args.Add(TEXT("ClassName"), ClassName);

		const auto Message = FText::Format(
			NSLOCTEXT(
				"UnrealEd",
				"CannotCreateBlueprintFromClass",
				"Cannot create a blueprint based on the class '{0}'."
			),
			Args
		);
		
		FMessageDialog::Open(
			EAppMsgType::Ok,
			Message
		);
		
		return false;
	}
	return true;
}

bool UGLibPythonEditorWidgetBlueprintFactory::CanCreateNew() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE
