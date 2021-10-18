// Copyright Gradess. All Rights Reserved.


#include "AssetTools/EditorAssetToolsBPLibrary.h"
#include "PackageTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/AssetRegistryInterface.h"


UPackage* UEditorAssetToolsBPLibrary::GetPackage(const FName& PackagePath)
{
	return FindPackage(nullptr, *PackagePath.ToString());
}

bool UEditorAssetToolsBPLibrary::UnloadPackage(const FName& PackagePath)
{
	const auto Package = FindPackage(nullptr, *PackagePath.ToString());
	return UPackageTools::UnloadPackages({Package});
}

bool UEditorAssetToolsBPLibrary::UnloadPackageAndDependencies(const FName& PackagePath)
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

bool UEditorAssetToolsBPLibrary::UnloadDependencies(
	TSet<FName>& Dependencies,
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

void UEditorAssetToolsBPLibrary::GetUnreferencedPackages(const TArray<FName>& PackagePaths, TSet<FName>& OutUnreferencedPackagePaths)
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

void UEditorAssetToolsBPLibrary::GetHardDependencies(const FName& PackagePath, TArray<FName>& OutDependencies)
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

void UEditorAssetToolsBPLibrary::GetHardReferencers(const FName& PackagePath, TArray<FName>& OutReferencers)
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

bool UEditorAssetToolsBPLibrary::IsProjectPackage(const FName& Package)
{
	return Package.ToString().StartsWith("/Game");
}

bool UEditorAssetToolsBPLibrary::IsAnybodyReferenced(const FName& PackagePath)
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
