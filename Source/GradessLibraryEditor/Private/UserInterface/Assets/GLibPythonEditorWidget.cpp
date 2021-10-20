// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Assets/GLibPythonEditorWidget.h"

#include "UserInterface/GLibEditorUserInterfaceLibrary.h"

void UGLibPythonEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// BUG: It doesn't work for nested python widgets
	UGLibEditorUserInterfaceLibrary::RegisterEditorWidget(this);
}

void UGLibPythonEditorWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	UGLibEditorUserInterfaceLibrary::UnregisterEditorWidget(this);
}
