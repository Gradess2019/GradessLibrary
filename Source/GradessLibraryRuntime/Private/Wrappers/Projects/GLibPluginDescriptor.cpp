// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibPluginDescriptor.h"
#include "PluginDescriptor.h"

#include "Wrappers/GLibConversions.h"

FGLibPluginDescriptor::FGLibPluginDescriptor()
{
	Version = 0;
	EnabledByDefault = EGLibPluginEnabledByDefault::Unspecified;
	bCanContainContent = false;
	bCanContainVerse = false;
	bIsBetaVersion = false;
	bIsExperimentalVersion = false;
	bInstalled = false;
	bRequiresBuildPlatform = false;
	bIsHidden = false;
	bExplicitlyLoaded = false;
	bHasExplicitPlatforms = false;
	bIsPluginExtension = false;
}

FGLibPluginDescriptor::FGLibPluginDescriptor(const FPluginDescriptor& InDescriptor)
{
	Version = InDescriptor.Version;
	VersionName = InDescriptor.VersionName;
	FriendlyName = InDescriptor.FriendlyName;
	Description = InDescriptor.Description;
	Category = InDescriptor.Category;
	CreatedBy = InDescriptor.CreatedBy;
	CreatedByURL = InDescriptor.CreatedByURL;
	DocsURL = InDescriptor.DocsURL;
	MarketplaceURL = InDescriptor.MarketplaceURL;
	SupportURL = InDescriptor.SupportURL;
	EngineVersion = InDescriptor.EngineVersion;
	EditorCustomVirtualPath = InDescriptor.EditorCustomVirtualPath;
	SupportedTargetPlatforms = InDescriptor.SupportedTargetPlatforms;
	SupportedPrograms = InDescriptor.SupportedPrograms;
	ParentPluginName = InDescriptor.ParentPluginName;
	EnabledByDefault = GLibConversions::ToBlueprintable<EGLibPluginEnabledByDefault>(InDescriptor.EnabledByDefault);
	bCanContainContent = InDescriptor.bCanContainContent;
	bCanContainVerse = InDescriptor.bCanContainVerse;
	bIsBetaVersion = InDescriptor.bIsBetaVersion;
	bIsExperimentalVersion = InDescriptor.bIsExperimentalVersion;
	bInstalled = InDescriptor.bInstalled;
	bRequiresBuildPlatform = InDescriptor.bRequiresBuildPlatform;
	bIsHidden = InDescriptor.bIsHidden;
	bExplicitlyLoaded = InDescriptor.bExplicitlyLoaded;
	bHasExplicitPlatforms = InDescriptor.bHasExplicitPlatforms;
	bIsPluginExtension = InDescriptor.bIsPluginExtension;

	GLibConversions::ToBlueprintable<FGLibModuleDescriptor>(InDescriptor.Modules, Modules);
}

FGLibPluginDescriptor::operator FPluginDescriptor() const
{
	FPluginDescriptor Descriptor;
	Descriptor.Version = Version;
	Descriptor.VersionName = VersionName;
	Descriptor.FriendlyName = FriendlyName;
	Descriptor.Description = Description;
	Descriptor.Category = Category;
	Descriptor.CreatedBy = CreatedBy;
	Descriptor.CreatedByURL = CreatedByURL;
	Descriptor.DocsURL = DocsURL;
	Descriptor.MarketplaceURL = MarketplaceURL;
	Descriptor.SupportURL = SupportURL;
	Descriptor.EngineVersion = EngineVersion;
	Descriptor.EditorCustomVirtualPath = EditorCustomVirtualPath;
	Descriptor.SupportedTargetPlatforms = SupportedTargetPlatforms;
	Descriptor.SupportedPrograms = SupportedPrograms;
	Descriptor.ParentPluginName = ParentPluginName;
	Descriptor.EnabledByDefault = GLibConversions::ToNative<EPluginEnabledByDefault>(EnabledByDefault);
	Descriptor.bCanContainContent = bCanContainContent;
	Descriptor.bCanContainVerse = bCanContainVerse;
	Descriptor.bIsBetaVersion = bIsBetaVersion;
	Descriptor.bIsExperimentalVersion = bIsExperimentalVersion;
	Descriptor.bInstalled = bInstalled;
	Descriptor.bRequiresBuildPlatform = bRequiresBuildPlatform;
	Descriptor.bIsHidden = bIsHidden;
	Descriptor.bExplicitlyLoaded = bExplicitlyLoaded;
	Descriptor.bHasExplicitPlatforms = bHasExplicitPlatforms;
	Descriptor.bIsPluginExtension = bIsPluginExtension;

	GLibConversions::ToNative<FModuleDescriptor>(Modules, Descriptor.Modules);

	return Descriptor;
}
