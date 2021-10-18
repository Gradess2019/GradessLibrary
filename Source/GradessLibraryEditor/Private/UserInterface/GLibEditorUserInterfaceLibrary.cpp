// Copyright Gradess. All Rights Reserved.


#include "UserInterface/GLibEditorUserInterfaceLibrary.h"

#include "AssetTools/GLibEditorAssetToolsLibrary.h"
#include "Blutility/Classes/EditorUtilityWidget.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"
#include "Blutility/Public/EditorUtilitySubsystem.h"
#include "Core/GLibFileToolsLibrary.h"

TSet<FString> UGLibEditorUserInterfaceLibrary::RegisteredWidgets = TSet<FString>();
TMap<FString, FDelegateHandle> UGLibEditorUserInterfaceLibrary::BoundWidgets = TMap<FString, FDelegateHandle>();
FDelegateHandle UGLibEditorUserInterfaceLibrary::SaveConfigDelegate = FDelegateHandle();

UGLibEditorUserInterfaceLibrary::UGLibEditorUserInterfaceLibrary()
{
	RebindSaveDelegate();
	LoadWidgetsFromConfig();
}

void UGLibEditorUserInterfaceLibrary::EnableAutoClosing(UEditorUtilityWidget* Widget)
{
	const auto WidgetBlueprintPath = GetWidgetBlueprintPath(Widget);
	const auto WidgetID = GetWidgetTabId(Widget);
	const auto Handle = GEditor->OnEditorClose().AddLambda(
		[WidgetID]
		{
			const auto EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
			EditorUtilitySubsystem->CloseTabByID(WidgetID);
		}
	);

	BoundWidgets.Add(WidgetBlueprintPath, Handle);
}

void UGLibEditorUserInterfaceLibrary::RegisterEditorWidget(UEditorUtilityWidget* Widget)
{
	if (!IsValid(Widget)) { return; }

	const auto ConfigPath = GetConfigPath();
	UGLibFileToolsLibrary::ValidateConfigFile(ConfigPath);

	const auto WidgetBlueprintPath = GetWidgetBlueprintPath(Widget);
	if (WidgetBlueprintPath.IsEmpty()) { return; }

	RegisteredWidgets.Add(WidgetBlueprintPath);
	SaveRegisteredWidgets();

	EnableAutoClosing(Widget);
	RebindSaveDelegate();
}

void UGLibEditorUserInterfaceLibrary::UnregisterEditorWidget(UEditorUtilityWidget* Widget)
{
	const auto Path = GetWidgetBlueprintPath(Widget);
	const auto Delegate = BoundWidgets.Find(Path);

	if (!Delegate) { return; }

	GEditor->OnEditorClose().Remove(*Delegate);
	RebindSaveDelegate();

	BoundWidgets.Remove(Path);
	RegisteredWidgets.Remove(Path);
}

void UGLibEditorUserInterfaceLibrary::SaveRegisteredWidgets()
{
	const auto ConfigPath = GetConfigPath();

	GConfig->SetArray(*GetSection(), *GetKey(), RegisteredWidgets.Array(), ConfigPath);
	GConfig->Flush(false, ConfigPath);
}

void UGLibEditorUserInterfaceLibrary::OpenWidgets()
{
	const auto EditorSubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	auto WidgetsToOpen = RegisteredWidgets.Array();

	for (const auto WidgetPath : WidgetsToOpen)
	{
		ReloadWidget(WidgetPath, EditorSubsystem);

		if (SpawnWidget(WidgetPath, EditorSubsystem)) { continue; }

		RegisteredWidgets.Remove(WidgetPath);
		SaveRegisteredWidgets();
	}
}

void UGLibEditorUserInterfaceLibrary::RebindSaveDelegate()
{
	GEditor->OnEditorClose().Remove(SaveConfigDelegate);
	SaveConfigDelegate = GEditor->OnEditorClose().AddStatic(&UGLibEditorUserInterfaceLibrary::SaveAndClear);
}

void UGLibEditorUserInterfaceLibrary::SaveAndClear()
{
	SaveRegisteredWidgets();
	RegisteredWidgets.Empty();
}

void UGLibEditorUserInterfaceLibrary::LoadWidgetsFromConfig()
{
	TArray<FString> Widgets;
	GConfig->GetArray(*GetSection(), *GetKey(), Widgets, GetConfigPath());
	RegisteredWidgets.Append(Widgets);
}

void UGLibEditorUserInterfaceLibrary::ReloadWidget(
	const FString& WidgetPath,
	UEditorUtilitySubsystem* EditorSubsystem
)
{
	const auto ID = GetWidgetTabIdByAssetName(WidgetPath);

	if (EditorSubsystem->DoesTabExist(ID)) { EditorSubsystem->CloseTabByID(ID); }

	UGLibEditorAssetToolsLibrary::ReloadAssetByPath(FName(WidgetPath));
}

bool UGLibEditorUserInterfaceLibrary::SpawnWidget(
	const FString& WidgetPath,
	UEditorUtilitySubsystem* EditorSubsystem
)
{
	const FStringAssetReference AssetReference(WidgetPath);
	const auto LoadedAsset = AssetReference.TryLoad();
	if (!LoadedAsset) { return false; }
	const auto WidgetBlueprint = CastChecked<UEditorUtilityWidgetBlueprint>(LoadedAsset);

	return EditorSubsystem->SpawnAndRegisterTab(WidgetBlueprint) != nullptr;
}

FName UGLibEditorUserInterfaceLibrary::GetWidgetTabId(const UEditorUtilityWidget* Widget)
{
	return Widget ? FName(Widget->GetClass()->GetPathName().LeftChop(2).Append("_ActiveTab")) : NAME_None;
}

FName UGLibEditorUserInterfaceLibrary::GetWidgetTabIdByAssetName(FString AssetName)
{
	return FName(AssetName.Append("_ActiveTab"));
}

UEditorUtilityWidgetBlueprint* UGLibEditorUserInterfaceLibrary::GetWidgetBlueprint(const UEditorUtilityWidget* Widget)
{
	const auto AssetReference = Widget->GetClass()->GetPathName().LeftChop(2);
	const FStringAssetReference Asset(AssetReference);
	if (!Asset.IsValid() || GIsSavingPackage || GIsGarbageCollecting) { return nullptr; }

	const auto WidgetBlueprint = Asset.TryLoad();
	return CastChecked<UEditorUtilityWidgetBlueprint>(WidgetBlueprint);
}

FString UGLibEditorUserInterfaceLibrary::GetWidgetBlueprintPath(const UEditorUtilityWidget* Widget)
{
	const auto Blueprint = GetWidgetBlueprint(Widget);
	return Blueprint ? Blueprint->GetPathName() : "";
}

FString UGLibEditorUserInterfaceLibrary::GetConfigPath()
{
	return FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir() / "RegisteredWidgets.ini");
}

FString UGLibEditorUserInterfaceLibrary::GetSection() { return TEXT("EditorWidgets"); }

FString UGLibEditorUserInterfaceLibrary::GetKey() { return TEXT("Widgets"); }
