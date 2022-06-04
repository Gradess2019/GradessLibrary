// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibLocalizationDescriptor.h"
#include "LocalizationDescriptor.h"

#include "Wrappers/GLibConversions.h"

FGLibLocalizationTargetDescriptor::FGLibLocalizationTargetDescriptor(FString InName, EGLibLocalizationTargetDescriptorLoadingPolicy InLoadingPolicy)
{
	Name = InName;
	LoadingPolicy = InLoadingPolicy;
}

FGLibLocalizationTargetDescriptor::FGLibLocalizationTargetDescriptor(const FLocalizationTargetDescriptor& InDescriptor)
{
	Name = InDescriptor.Name;
	LoadingPolicy = GLibConversions::ToBlueprintable<EGLibLocalizationTargetDescriptorLoadingPolicy>(InDescriptor.LoadingPolicy);
}

FGLibLocalizationTargetDescriptor::operator FLocalizationTargetDescriptor() const
{
	FLocalizationTargetDescriptor Descriptor;
	Descriptor.Name = Name;
	Descriptor.LoadingPolicy = GLibConversions::ToNative<ELocalizationTargetDescriptorLoadingPolicy::Type>(LoadingPolicy);
	
	return Descriptor;
}
