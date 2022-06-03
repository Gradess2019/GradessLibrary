// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibModuleDescriptor.h"

#include "ModuleDescriptor.h"

#include "Wrappers/GLibConversions.h"

FGLibModuleDescriptor::FGLibModuleDescriptor(const FName InName, EGLibHostType InType, EGLibLoadingPhase InLoadingPhase)
{
	Name = InName;
	Type = InType;
	LoadingPhase = InLoadingPhase;
	bHasExplicitPlatforms = false;
}

FGLibModuleDescriptor::FGLibModuleDescriptor(const FModuleDescriptor& InDescriptor)
{
	Name = InDescriptor.Name;
	Type = GLibConversions::ToBlueprintable<EGLibHostType>(InDescriptor.Type);
	LoadingPhase = GLibConversions::ToBlueprintable<EGLibLoadingPhase>(InDescriptor.LoadingPhase);
	PlatformAllowList = InDescriptor.PlatformAllowList;
	PlatformDenyList = InDescriptor.PlatformDenyList;
	TargetAllowList = GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetAllowList);
	TargetDenyList = GLibConversions::ToBlueprintable<EGLibBuildTargetType>(InDescriptor.TargetDenyList);
	TargetConfigurationAllowList = GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationAllowList);
	TargetConfigurationDenyList = GLibConversions::ToBlueprintable<EGLibBuildConfiguration>(InDescriptor.TargetConfigurationDenyList);
	ProgramAllowList = InDescriptor.ProgramAllowList;
	ProgramDenyList = InDescriptor.ProgramDenyList;
	AdditionalDependencies = InDescriptor.AdditionalDependencies;
	bHasExplicitPlatforms = InDescriptor.bHasExplicitPlatforms;
}

FGLibModuleDescriptor::operator FModuleDescriptor() const
{
	FModuleDescriptor ModuleDescriptor;
	ModuleDescriptor.Name = Name;
	ModuleDescriptor.Type = GLibConversions::ToNative<EHostType::Type>(Type);
	ModuleDescriptor.LoadingPhase = GLibConversions::ToNative<ELoadingPhase::Type>(LoadingPhase);
	ModuleDescriptor.PlatformAllowList = PlatformAllowList;
	ModuleDescriptor.PlatformDenyList = PlatformDenyList;
	ModuleDescriptor.TargetAllowList = GLibConversions::ToNative<EBuildTargetType>(TargetAllowList);
	ModuleDescriptor.TargetDenyList = GLibConversions::ToNative<EBuildTargetType>(TargetDenyList);
	ModuleDescriptor.TargetConfigurationAllowList = GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationAllowList);
	ModuleDescriptor.TargetConfigurationDenyList = GLibConversions::ToNative<EBuildConfiguration>(TargetConfigurationDenyList);
	ModuleDescriptor.ProgramAllowList = ProgramAllowList;
	ModuleDescriptor.ProgramDenyList = ProgramDenyList;
	ModuleDescriptor.AdditionalDependencies = AdditionalDependencies;
	ModuleDescriptor.bHasExplicitPlatforms = bHasExplicitPlatforms;

	return ModuleDescriptor;
}
