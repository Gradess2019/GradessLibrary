// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserDelegates.h"
#include "IContentBrowserSingleton.h"
#include "Components/Widget.h"
#include "UserInterface/Structs/GLibAssetThumbnail.h"
#include "UserInterface/Structs/GLibContentBrowserExtras.h"
#include "GLibMultiAssetPicker.generated.h"

#pragma region Forward declarations
struct FAssetPickerConfig;
#pragma endregion Forward declarations

#pragma region Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnAssetSelected, const FAssetData&, AssetData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnPathSelected, const FString&, Path);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnAssetDoubleClicked, const FAssetData&, AssetData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGLibOnAssetEnterPressed, const TArray<FAssetData>&, SelectedAssets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGLibOnAssetToolTipClosing);
#pragma endregion Delegate declarations

/** 
 * @brief A struct containing details about how the asset picker should behave
 */
USTRUCT(
	BlueprintType
)
struct GRADESSLIBRARYEDITOR_API FGLibMultiAssetPickerConfig
{
	GENERATED_BODY()

	/**
	 * @brief Asset picker selection mode
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TEnumAsByte<ESelectionMode::Type> SelectionMode;

	/** 
	 * @brief The contents of the label on the thumbnail
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TEnumAsByte<EGLibThumbnailLabel> ThumbnailLabel;

	/** 
	 * @brief The default view mode
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TEnumAsByte<EGLibAssetViewType> InitialAssetViewType;

	/** 
	 * @brief The default scale for thumbnails. [0-1] range
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		meta = (
			UIMin = "0",
			UIMax = "1",
			ClampMin = "0",
			ClampMax = "1"
		)
	)
	float ThumbnailScale;

	/** 
	 * @brief If true, the search box will gain focus when the asset picker is created 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bFocusSearchBoxWhenOpened;

	/** 
	 * @brief If true, a "None" item will always appear at the top of the list 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bAllowNullSelection;

	/** 
	 * @brief If true, show the bottom toolbar which shows # of assets selected, view mode buttons, etc... 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bShowBottomToolbar;

	/** 
	 * @brief If false, auto-hide the search bar above 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bAutohideSearchBar;

	/** 
	 * @brief Whether to allow dragging of items 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bAllowDragging;

	/** 
	 * @brief Indicates if this view is allowed to show classes 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bCanShowClasses;

	/** 
	 * @brief Indicates if the 'Show folders' option should be enabled or disabled 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bCanShowFolders;

	/** 
	 * @brief Indicates if the 'Real-Time Thumbnails' option should be enabled or disabled 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bCanShowRealTimeThumbnails;

	/** 
	 * @brief Indicates if the 'Show Developers' option should be enabled or disabled 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bCanShowDevelopersFolder;

	/** 
	 * @brief Indicates if engine content should always be shown 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bForceShowEngineContent;

	/** 
	 * @brief Indicates if plugin content should always be shown 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bForceShowPluginContent;

	/** 
	 * @brief Indicates if the context menu is going to load the assets, and if so to preload before the context menu
	 * is shown, and warn about the pending load. 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bPreloadAssetsForContextMenu;

	/** 
	 * @brief Indicates that we would like to build the filter UI with the Asset Picker 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bAddFilterUI;

	/** 
	 * @brief If true, show path in column view 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bShowPathInColumnView;
	/** 
	 * @brief If true, show class in column view 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bShowTypeInColumnView;
	/** 
	 * @brief If true, sort by path in column view. Only works if initial view type is Column 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bSortByPathInColumnView;

	/** 
	 * @brief Override the default filter context menu layout 
	 */
	EAssetTypeCategories::Type DefaultFilterMenuExpansion;

	/** 
	 * @brief The filter component for package names 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TArray<FName> PackageNames;

	/** 
	 * @brief The filter component for package paths 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TArray<FName> PackagePaths;

	/** 
	 * @brief The filter component containing specific object paths 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TArray<FName> ObjectPaths;

	/** 
	 * @brief The filter component for class names. Instances of the specified classes, but not subclasses (by default),
	 * will be included. Derived classes will be included only if bRecursiveClasses is true. 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TArray<FName> ClassNames;

	/** On
	 * @brief ly if bRecursiveClasses is true, the results will exclude classes (and subclasses) in this list 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TSet<FName> RecursiveClassesExclusionSet;

	/** 
	 * @brief If true, PackagePath components will be recursive 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bRecursivePaths;

	/** 
	 * @brief If true, subclasses of ClassNames will also be included and RecursiveClassesExclusionSet will be excluded. 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bRecursiveClasses;

	/** 
	 * @brief If true, only on-disk assets will be returned. Be warned that this is rarely what you want and
	 * should only be used for performance reasons 
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	bool bIncludeOnlyOnDiskAssets;

	FGLibMultiAssetPickerConfig() :
		SelectionMode(ESelectionMode::Multi),
		ThumbnailLabel(EThumbnailLabel::ClassName),
		InitialAssetViewType(EAssetViewType::Tile),
		ThumbnailScale(0.1f),
		bFocusSearchBoxWhenOpened(true),
		bAllowNullSelection(false),
		bShowBottomToolbar(true),
		bAutohideSearchBar(false),
		bAllowDragging(true),
		bCanShowClasses(true),
		bCanShowFolders(false),
		bCanShowRealTimeThumbnails(false),
		bCanShowDevelopersFolder(true),
		bForceShowEngineContent(false),
		bForceShowPluginContent(false),
		bPreloadAssetsForContextMenu(true),
		bAddFilterUI(false),
		bShowPathInColumnView(false),
		bShowTypeInColumnView(true),
		bSortByPathInColumnView(false),
		DefaultFilterMenuExpansion(EAssetTypeCategories::Basic),
		bRecursivePaths(false),
		bRecursiveClasses(false),
		bIncludeOnlyOnDiskAssets(false) {}

	FORCEINLINE operator FAssetPickerConfig() const
	{
		FAssetPickerConfig NewConfig;
		NewConfig.SelectionMode = SelectionMode;
		NewConfig.ThumbnailLabel = static_cast<EThumbnailLabel::Type>(ThumbnailLabel.GetValue());
		NewConfig.ThumbnailScale = ThumbnailScale;
		NewConfig.InitialAssetViewType = static_cast<EAssetViewType::Type>(InitialAssetViewType.GetValue());
		NewConfig.bFocusSearchBoxWhenOpened = bFocusSearchBoxWhenOpened;
		NewConfig.bAllowNullSelection = bAllowNullSelection;
		NewConfig.bShowBottomToolbar = bShowBottomToolbar;
		NewConfig.bAutohideSearchBar = bAutohideSearchBar;
		NewConfig.bAllowDragging = bAllowDragging;
		NewConfig.bCanShowClasses = bCanShowClasses;
		NewConfig.bCanShowFolders = bCanShowFolders;
		NewConfig.bCanShowRealTimeThumbnails = bCanShowRealTimeThumbnails;
		NewConfig.bCanShowDevelopersFolder = bCanShowDevelopersFolder;
		NewConfig.bForceShowEngineContent = bForceShowEngineContent;
		NewConfig.bForceShowPluginContent = bForceShowPluginContent;
		NewConfig.bPreloadAssetsForContextMenu = bPreloadAssetsForContextMenu;
		NewConfig.bAddFilterUI = bAddFilterUI;
		NewConfig.bShowPathInColumnView = bShowPathInColumnView;
		NewConfig.bShowTypeInColumnView = bShowTypeInColumnView;
		NewConfig.bSortByPathInColumnView = bSortByPathInColumnView;
		NewConfig.DefaultFilterMenuExpansion = DefaultFilterMenuExpansion;

		FARFilter Filter;
		Filter.PackageNames = PackageNames;
		Filter.PackagePaths = PackagePaths;
		Filter.ObjectPaths = ObjectPaths;
		Filter.ClassNames = ClassNames;
		Filter.RecursiveClassesExclusionSet = RecursiveClassesExclusionSet;
		Filter.bRecursivePaths = bRecursivePaths;
		Filter.bRecursiveClasses = bRecursiveClasses;

		NewConfig.Filter = Filter;
		return NewConfig;
	}
};

/**
 * @brief Compact content browser for multiple assets picking
 */
UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	)
)
class GRADESSLIBRARYEDITOR_API UGLibMultiAssetPicker : public UWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Executes when asset was selected
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibOnAssetSelected OnAssetSelected;

	/**
	 * @brief Executes when directory was selected
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibOnPathSelected OnPathSelected;

	/**
	 * @brief Executes when asset was double clicked
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibOnAssetDoubleClicked OnAssetDoubleClicked;

	/**
	 * @brief Executes when "Enter" key was pressed on asset
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibOnAssetEnterPressed OnAssetEnterPressed;

	/**
	 * @brief Executes when tool tip is closing, usually on unhover event
	 */
	UPROPERTY(
		BlueprintAssignable,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibOnAssetToolTipClosing OnAssetToolTipClosing;

protected:
	/**
	* @brief Current asset picker config
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "GLib|MultiAssetPicker"
	)
	FGLibMultiAssetPickerConfig Config;

	/**
	 * @brief Used for obtaining current selected assets
	 */
	FGetCurrentSelectionDelegate GetCurrentSelectionDelegate;
	
	/**
	 * @brief Used for selecting assets programmatically 
	 */
	FSyncToAssetsDelegate SelectAssetsDelegate;
	
	/**
	 * @brief Used for setting new asset picker filter
	 */
	FSetARFilterDelegate SetARFilterDelegate;

	/**
	 * @brief Current asset picker instance
	 */
	TSharedPtr<class SAssetPicker> AssetPicker;

public:
	/**
	 * @brief Sets new filter for asset picker
	 * @param NewFilter filter to set
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|MultiAssetPicker"
	)
	void SetFilter(const FARFilter& NewFilter);

	/**
	 * @brief Selects assets in asset picker
	 * @param AssetsData assets to select
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|MultiAssetPicker"
	)
	void SelectAssets(const TArray<FAssetData>& AssetsData);

	/**
	 * @brief Returns array of selected assets
	 * @param OutSelectedAssets array of selected assets
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|MultiAssetPicker"
	)
	void GetSelectedAssets(UPARAM(DisplayName = "SelectedAssets") TArray<FAssetData>& OutSelectedAssets) const;
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual TSharedRef<SWidget> CreateAssetPicker(const FAssetPickerConfig& AssetPickerConfig);

private:
	/**
	 * @brief Setup delegates for asset picker config
	 * @param AssetPickerConfig config to setup
	 */
	void SetupDelegates(FAssetPickerConfig& AssetPickerConfig);

};
