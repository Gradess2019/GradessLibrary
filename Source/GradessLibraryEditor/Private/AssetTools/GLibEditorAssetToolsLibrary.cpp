// Copyright Gradess. All Rights Reserved.


#include "AssetTools/GLibEditorAssetToolsLibrary.h"

#include "AdvancedCopyCustomization.h"
#include "AssetToolsModule.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/AssetRegistryInterface.h"


UPackage* UGLibEditorAssetToolsLibrary::GetPackage(const FName& PackagePath)
{
	return FindPackage(nullptr, *PackagePath.ToString());
}

bool UGLibEditorAssetToolsLibrary::UnloadPackage(const FName& PackagePath)
{
	const auto Package = FindPackage(nullptr, *PackagePath.ToString());
	return UPackageTools::UnloadPackages({Package});
}

bool UGLibEditorAssetToolsLibrary::UnloadPackageAndDependencies(const FName& PackagePath)
{
	if (!UnloadPackage(PackagePath))
	{
		return false;
	}

	auto bSuccess = true;

	TArray<FName> Dependencies;
	GetHardDependencies(PackagePath, Dependencies);

	TSet<FName> PackagesToUnload;
	GetUnreferencedPackages(Dependencies, PackagesToUnload);

	// Unload till we have loaded dependencies
	while (PackagesToUnload.Num() > 0)
	{
		TSet<FName> ChildDependencies;
		bSuccess = bSuccess && UnloadDependencies(PackagesToUnload, ChildDependencies);

		PackagesToUnload = ChildDependencies;
	}

	return bSuccess;
}

bool UGLibEditorAssetToolsLibrary::UnloadDependencies(
	const TSet<FName>& Dependencies,
	TSet<FName>& OutChildDependencies
)
{
	TArray<UPackage*> Packages;
	for (auto& Dependency : Dependencies)
	{
		const auto Package = FindPackage(nullptr, *Dependency.ToString());
		Packages.Add(Package);
	}

	const auto bSuccess = UPackageTools::UnloadPackages(Packages);
	if (!bSuccess)
	{
		return false;
	}

	for (auto& Dependency : Dependencies)
	{
		TArray<FName> ChildDependencies;
		GetHardDependencies(Dependency, ChildDependencies);

		TSet<FName> UnreferencedChildDependencies;
		GetUnreferencedPackages(ChildDependencies, UnreferencedChildDependencies);

		OutChildDependencies.Append(UnreferencedChildDependencies);
	}

	return true;
}

void UGLibEditorAssetToolsLibrary::GetUnreferencedPackages(const TArray<FName>& PackagePaths,
                                                         TSet<FName>& OutUnreferencedPackagePaths)
{
	for (auto& Package : PackagePaths)
	{
		if (!IsProjectPackage(Package) || IsAnybodyReferenced(Package))
		{
			continue;
		}

		GLog->Logf(TEXT("Unreferenced package: %s"), *Package.ToString());
		OutUnreferencedPackagePaths.Add(Package);
	}
}

void UGLibEditorAssetToolsLibrary::GetHardDependencies(const FName& PackagePath, TArray<FName>& OutDependencies)
{
	const auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	AssetRegistry.Get().GetDependencies(
		PackagePath,
		OutDependencies,
		UE::AssetRegistry::EDependencyCategory::Package,
		UE::AssetRegistry::FDependencyQuery(
			UE::AssetRegistry::EDependencyQuery::Hard
		)
	);
}

void UGLibEditorAssetToolsLibrary::GetHardReferencers(const FName& PackagePath, TArray<FName>& OutReferencers)
{
	const auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	AssetRegistry.GetReferencers(
		PackagePath,
		OutReferencers,
		UE::AssetRegistry::EDependencyCategory::Package,
		UE::AssetRegistry::FDependencyQuery(
			UE::AssetRegistry::EDependencyQuery::Hard
		)
	);
}

bool UGLibEditorAssetToolsLibrary::IsProjectPackage(const FName& Package)
{
	return Package.ToString().StartsWith("/Game");
}

bool UGLibEditorAssetToolsLibrary::IsAnybodyReferenced(const FName& PackagePath)
{
	const auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();

	TArray<FName> Referencers;
	GetHardReferencers(PackagePath, Referencers);

	for (const auto& Referencer : Referencers)
	{
		auto AssetData = AssetRegistry.GetAssetByObjectPath(Referencer);
		if (AssetData.IsAssetLoaded())
		{
			return true;
		}
	}
	return false;
}

void UGLibEditorAssetToolsLibrary::AdvancedCopyDirectory(
	const FString Source,
	const FString Destination,
	const FGLibAdvancedCopyParams& Params
)
{
	const auto AssetTools = &FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	const auto SourceName = TArray<FName>({*Source});

	auto NativeParams = FAdvancedCopyParams(SourceName, Destination);
	Params.CopyToNative(NativeParams);

	const auto CopyCustomization =
		UAdvancedCopyCustomization::StaticClass()->GetDefaultObject<UAdvancedCopyCustomization>();

	NativeParams.AddCustomization(CopyCustomization);
	AssetTools->InitAdvancedCopyFromCopyParams(NativeParams);
}

void UGLibEditorAssetToolsLibrary::ReloadAsset(const FAssetData& AssetData)
{
	if (!AssetData.IsValid()) { return; }
	
	const auto Package = AssetData.GetPackage();
	const auto Packages = TArray<UPackage*>({Package});

	UPackageTools::ReloadPackages(Packages);
}

void UGLibEditorAssetToolsLibrary::ReloadAssetByPath(const FName& AssetPath)
{
	const auto& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	const auto Asset = AssetRegistry.GetAssetByObjectPath(AssetPath);

	if (!Asset.IsValid()) { return; }

	ReloadAsset(Asset);
}

void UGLibEditorAssetToolsLibrary::ReloadAssets(const TArray<FAssetData>& AssetsData)
{
	auto Packages = TArray<UPackage*>();
	Packages.Reserve(AssetsData.Num());

	for (auto AssetData : AssetsData)
	{
		if (!AssetData.IsValid()) { continue; }

		const auto NewPackage = AssetData.GetPackage();
		Packages.Add(NewPackage);
	}

	UPackageTools::ReloadPackages(Packages);
}
