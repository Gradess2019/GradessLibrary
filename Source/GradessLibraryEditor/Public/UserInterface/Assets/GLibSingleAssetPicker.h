// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "GLibSingleAssetPicker.generated.h"

#pragma region Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnSetObject, const FAssetData&, AssetData);
#pragma endregion Delegate declarations

/**
 * @brief Config struct for USingleAssetPicker
 */
USTRUCT(
	BlueprintType
)
struct FGLibSingleAssetPickerConfig
{
	GENERATED_BODY()

	/**
	 * @brief Class that is allowed in the asset picker
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	UClass* AllowedClass;
	
	/**
	* @brief Whether the asset can be 'None'
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool AllowClear;

	/**
	 * @brief Whether to show the 'Use Selected' button
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool DisplayUseSelected;

	/**
	 * @brief Whether to show the 'Browse' button
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool DisplayBrowse;

	/**
	 * @brief Whether to enable the content Picker
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool EnableContentPicker;

	/**
	 * @brief Whether or not to display a smaller, compact size for the asset thumbnail
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool DisplayCompactSize;

	/**
	 * @brief Whether or not to display the asset thumbnail
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool DisplayThumbnail;

	/**
	 * @brief Custom thumbnail size
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	FIntPoint ThumbnailSizeOverride;
	
	FGLibSingleAssetPickerConfig(
		UClass* const AllowedClass = UObject::StaticClass(),
		const bool bAllowClear = true,
		const bool bDisplayUseSelected = true,
		const bool bDisplayBrowse = true,
		const bool bEnableContentPicker = true,
		const bool bDisplayCompactSize = false,
		const bool bDisplayThumbnail = true,
		const FIntPoint& ThumbnailSizeOverride = FIntPoint(64, 64)
	) :
		AllowedClass(AllowedClass),
		AllowClear(bAllowClear),
		DisplayUseSelected(bDisplayUseSelected),
		DisplayBrowse(bDisplayBrowse),
		EnableContentPicker(bEnableContentPicker),
		DisplayCompactSize(bDisplayCompactSize),
		DisplayThumbnail(bDisplayThumbnail),
		ThumbnailSizeOverride(ThumbnailSizeOverride) {}
};

/**
 * @brief Widget for picking asset or actor 
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	)
)
class GRADESSLIBRARYEDITOR_API UGLibSingleAssetPicker : public UWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Executes when asset was selected
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|SingleAssetPicker"
	)
	FGLibOnSetObject OnAssetSelected;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	/**
	 * @brief Returns current selected asset path
	 * @return path to asset
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|SingleAssetPicker"
	)
	FString GetSelectedAssetPath() const;

	/**
	 * @brief Handles event when user select asset
	 * @param AssetData selected asset
	 */
	UFUNCTION(
		BlueprintNativeEvent,
		Category = "GLib|SingleAssetPicker"
	)
	void AssetSelected(const FAssetData& AssetData);

	/**
	 * @brief Checks if an asset should be set
	 * @param AssetData asset data to check
	 * @return true if yes, false otherwise
	 */
	UFUNCTION(
		BlueprintNativeEvent,
		BlueprintPure,
		Category = "GLib|SingleAssetPicker"
	)
	bool ShouldSetAsset(const FAssetData& AssetData);

	/**
	 * @brief Checks if an asset is valid to use
	 * @param AssetData asset data to check
	 * @return true if asset is valid, false otherwise
	 */
	UFUNCTION(
		BlueprintNativeEvent,
		BlueprintPure,
		Category = "GLib|SingleAssetPicker"
	)
	bool ShouldFilterAsset(const FAssetData& AssetData);

	/**
	 * @brief Checks if the asset picker should be enabled
	 * @return true if yes, false otherwise
	 */
	UFUNCTION(
		BlueprintNativeEvent,
		BlueprintPure,
		Category = "GLib|SingleAssetPicker"
	)
	bool IsEnabled();

protected:
	/**
	 * @brief Current asset picker config
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "GLib|SingleAssetPicker"
	)
	FGLibSingleAssetPickerConfig Config;

	/**
	 * @brief Current selected asset
	 */
	UPROPERTY(
		BlueprintReadOnly,
		Category = "GLib|SingleAssetPicker"
	)
	FAssetData SelectedAsset;

	/**
	 * @brief Pointer to current asset picker instance
	 */
	TSharedPtr<SWidget> Picker;
	
	/**
	 * @brief Pointer to current asset picker thumbnail pool instance
	 */
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool;
};
