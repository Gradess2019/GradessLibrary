// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibPluginReferenceDescriptor.h"

#include "Wrappers/GLibConversions.h"

FGLibPluginReferenceDescriptor::FGLibPluginReferenceDescriptor(const FString& InName, bool bInEnabled)
{
	Name = InName;
	bEnabled = bInEnabled;
	bOptional = false;
	bHasExplicitPlatforms = false;
}

FGLibPluginReferenceDescriptor::FGLibPluginReferenceDescriptor(const FPluginReferenceDescriptor& InDescriptor)
{
	Name = InDescriptor.Name;
	bEnabled = InDescriptor.bEnabled;
	bOptional = InDescriptor.bOptional;
	Description = InDescriptor.Description;
	MarketplaceURL = InDescriptor.MarketplaceURL;
	PlatformAllowList = InDescriptor.PlatformAllowList;
	PlatformDenyList = InDescriptor.PlatformDenyList;
	TargetConfigurationAllowList = GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationAllowList);
	TargetConfigurationDenyList = GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationDenyList);
	TargetAllowList = GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetAllowList);
	TargetDenyList = GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetDenyList);
	SupportedTargetPlatforms = InDescriptor.SupportedTargetPlatforms;
	bHasExplicitPlatforms = InDescriptor.bHasExplicitPlatforms;
}

FGLibPluginReferenceDescriptor::operator FPluginReferenceDescriptor() const
{
	FPluginReferenceDescriptor Descriptor;
	Descriptor.Name = Name;
	Descriptor.bEnabled = bEnabled;
	Descriptor.bOptional = bOptional;
	Descriptor.Description = Description;
	Descriptor.MarketplaceURL = MarketplaceURL;
	Descriptor.PlatformAllowList = PlatformAllowList;
	Descriptor.PlatformDenyList = PlatformDenyList;
	Descriptor.TargetConfigurationAllowList = GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationAllowList);
	Descriptor.TargetConfigurationDenyList = GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationDenyList);
	Descriptor.TargetAllowList = GLibConversions::ToNative<EBuildTargetType>(TargetAllowList);
	Descriptor.TargetDenyList = GLibConversions::ToNative<EBuildTargetType>(TargetDenyList);
	Descriptor.SupportedTargetPlatforms = SupportedTargetPlatforms;
	Descriptor.bHasExplicitPlatforms = bHasExplicitPlatforms;

	return Descriptor;
}
