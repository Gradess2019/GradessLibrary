// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Assets/GLibMultiAssetPicker.h"
#include "ContentBrowserModule.h"
#include "ContentBrowser/Private/SAssetPicker.h"

#define LOCTEXT_NAMESPACE "UGLibMultiAssetPicker"

void UGLibMultiAssetPicker::SetFilter(const FARFilter& NewFilter)
{
	SetARFilterDelegate.Execute(NewFilter);
}

void UGLibMultiAssetPicker::SelectAssets(const TArray<FAssetData>& AssetsData)
{
	SelectAssetsDelegate.Execute(AssetsData);
}

void UGLibMultiAssetPicker::GetSelectedAssets(TArray<FAssetData>& OutSelectedAssets) const
{
	OutSelectedAssets = GetCurrentSelectionDelegate.Execute();
}

void UGLibMultiAssetPicker::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	AssetPicker.Reset();
}

TSharedRef<SWidget> UGLibMultiAssetPicker::RebuildWidget()
{
	FAssetPickerConfig AssetPickerConfig = Config;
	AssetPickerConfig.AssetShowWarningText = LOCTEXT("NoItems_Warning", "No Items Found");
	
	SetupDelegates(AssetPickerConfig);

	return CreateAssetPicker(AssetPickerConfig);
}

TSharedRef<SWidget> UGLibMultiAssetPicker::CreateAssetPicker(const FAssetPickerConfig& AssetPickerConfig)
{
	IContentBrowserSingleton& ContentBrowser =
		FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();
	
	TSharedRef<SWidget> AssetPickerRef = ContentBrowser.CreateAssetPicker(AssetPickerConfig);
	AssetPicker = StaticCastSharedRef<SAssetPicker>(AssetPickerRef);
	return AssetPickerRef;
}

void UGLibMultiAssetPicker::SetupDelegates(FAssetPickerConfig& AssetPickerConfig) {
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateRaw(&OnAssetSelected, &FGLibOnAssetSelected::Broadcast);
	AssetPickerConfig.OnFolderEntered = FOnPathSelected::CreateRaw(&OnPathSelected, &FGLibOnPathSelected::Broadcast);
	AssetPickerConfig.OnAssetDoubleClicked = FOnAssetDoubleClicked::CreateRaw(&OnAssetDoubleClicked, &FGLibOnAssetDoubleClicked::Broadcast);
	AssetPickerConfig.OnAssetEnterPressed = FOnAssetEnterPressed::CreateRaw(&OnAssetEnterPressed, &FGLibOnAssetEnterPressed::Broadcast);
	AssetPickerConfig.OnAssetToolTipClosing = FOnAssetToolTipClosing::CreateRaw(&OnAssetToolTipClosing, &FGLibOnAssetToolTipClosing::Broadcast);
	
	AssetPickerConfig.GetCurrentSelectionDelegates.Add(&GetCurrentSelectionDelegate);
	AssetPickerConfig.SyncToAssetsDelegates.Add(&SelectAssetsDelegate);
	AssetPickerConfig.SetFilterDelegates.Add(&SetARFilterDelegate);
}

#undef LOCTEXT_NAMESPACE
