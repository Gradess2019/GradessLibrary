// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Assets/GLibPythonEditorWidget.h"

#include "UserInterface/GLibEditorUserInterfaceLibrary.h"

void UGLibPythonEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGLibEditorUserInterfaceLibrary::RegisterEditorWidget(this);
}

void UGLibPythonEditorWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	UGLibEditorUserInterfaceLibrary::UnregisterEditorWidget(this);
}
