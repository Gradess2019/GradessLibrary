// Copyright Gradess. All Rights Reserved.


#include "UserInterface/Assets/GLibSingleAssetPicker.h"
#include "PropertyCustomizationHelpers.h"

TSharedRef<SWidget> UGLibSingleAssetPicker::RebuildWidget()
{
	ThumbnailPool = MakeShareable(new FAssetThumbnailPool(2));

	FOnSetObject OnSetObject;
	Picker = SNew(SObjectPropertyEntryBox)
		.ObjectPath_UObject(this, &UGLibSingleAssetPicker::GetSelectedAssetPath)
		.ThumbnailPool(ThumbnailPool)
		.AllowedClass(Config.AllowedClass)
		.OnShouldSetAsset(FOnShouldSetAsset::CreateUObject(this, &UGLibSingleAssetPicker::ShouldSetAsset))
		.OnObjectChanged(FOnSetObject::CreateUObject(this, &UGLibSingleAssetPicker::AssetSelected))
		.OnShouldFilterAsset(FOnShouldFilterAsset::CreateUObject(this, &UGLibSingleAssetPicker::ShouldFilterAsset))
		.OnIsEnabled(FOnIsEnabled::CreateUObject(this, &UGLibSingleAssetPicker::IsEnabled))
		.AllowClear(Config.AllowClear)
		.DisplayUseSelected(Config.DisplayUseSelected)
		.DisplayBrowse(Config.DisplayBrowse)
		.EnableContentPicker(Config.EnableContentPicker)
		.DisplayCompactSize(Config.DisplayCompactSize)
		.DisplayThumbnail(Config.DisplayThumbnail)
		.ThumbnailSizeOverride(Config.ThumbnailSizeOverride);
	
	return Picker.ToSharedRef();
}

void UGLibSingleAssetPicker::ReleaseSlateResources(bool bReleaseChildren)
{
	Picker.Reset();
	ThumbnailPool.Reset();
}

FString UGLibSingleAssetPicker::GetSelectedAssetPath() const
{
	return SelectedAsset.IsValid() ? SelectedAsset.ObjectPath.ToString() : "";
}

bool UGLibSingleAssetPicker::ShouldSetAsset_Implementation(const FAssetData& AssetData)
{
	return false;
}

bool UGLibSingleAssetPicker::ShouldFilterAsset_Implementation(const FAssetData& AssetData)
{
	return false;
}

bool UGLibSingleAssetPicker::IsEnabled_Implementation()
{
	return true;
}

void UGLibSingleAssetPicker::AssetSelected_Implementation(const FAssetData& AssetData)
{
	SelectedAsset = AssetData;
	OnAssetSelected.Broadcast(SelectedAsset);
}

