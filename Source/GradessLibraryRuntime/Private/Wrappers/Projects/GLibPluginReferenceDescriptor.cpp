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
	SupportedTargetPlatforms = InDescriptor.SupportedTargetPlatforms;
	bHasExplicitPlatforms = InDescriptor.bHasExplicitPlatforms;
	
	GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationAllowList, TargetConfigurationAllowList);
	GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationDenyList, TargetConfigurationDenyList);
	GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetAllowList, TargetAllowList);
	GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetDenyList, TargetDenyList);
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
	Descriptor.SupportedTargetPlatforms = SupportedTargetPlatforms;
	Descriptor.bHasExplicitPlatforms = bHasExplicitPlatforms;

	GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationAllowList, Descriptor.TargetConfigurationAllowList);
	GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationDenyList, Descriptor.TargetConfigurationDenyList);
	GLibConversions::ToNative<EBuildTargetType>(TargetAllowList, Descriptor.TargetAllowList);
	GLibConversions::ToNative<EBuildTargetType>(TargetDenyList, Descriptor.TargetDenyList);
	
	return Descriptor;
}
