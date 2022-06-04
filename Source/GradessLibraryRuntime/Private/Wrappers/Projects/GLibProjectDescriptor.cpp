// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibProjectDescriptor.h"
#include "ProjectDescriptor.h"
#include "Wrappers/GLibConversions.h"
#include "Wrappers/Projects/GLibPluginReferenceDescriptor.h"

FGLibProjectDescriptor::FGLibProjectDescriptor()
{
	FileVersion = EGLibProjectDescriptorVersion::Latest;
	EpicSampleNameHash = 0;
	bIsEnterpriseProject = false;
	bDisableEnginePluginsByDefault = false;
}

FGLibProjectDescriptor::FGLibProjectDescriptor(const FProjectDescriptor& InDescriptor)
{
	FileVersion = GLibConversions::ToBlueprintable<EGLibProjectDescriptorVersion>(InDescriptor.FileVersion);
	EngineAssociation = InDescriptor.EngineAssociation;
	Category = InDescriptor.Category;
	Description = InDescriptor.Description;
	Modules = GLibConversions::ToBlueprintable<FGLibModuleDescriptor>(InDescriptor.Modules);
	Plugins = GLibConversions::ToBlueprintable<FGLibPluginReferenceDescriptor>(InDescriptor.Plugins);
	TargetPlatforms = InDescriptor.TargetPlatforms;
	EpicSampleNameHash = InDescriptor.EpicSampleNameHash;
	PreBuildSteps = InDescriptor.PreBuildSteps;
	PostBuildSteps = InDescriptor.PostBuildSteps;
	bIsEnterpriseProject = InDescriptor.bIsEnterpriseProject;
	bDisableEnginePluginsByDefault = InDescriptor.bDisableEnginePluginsByDefault;
	AdditionalPluginDirectories = InDescriptor.GetAdditionalPluginDirectories();
	AdditionalRootDirectories = InDescriptor.GetAdditionalRootDirectories();
}

FGLibProjectDescriptor::operator FProjectDescriptor() const
{
	FProjectDescriptor Descriptor;
	Descriptor.FileVersion = GLibConversions::ToNative<EProjectDescriptorVersion::Type>(FileVersion);
	Descriptor.EngineAssociation = EngineAssociation;
	Descriptor.Category = Category;
	Descriptor.Description = Description;
	Descriptor.Modules = GLibConversions::ToNative<FModuleDescriptor>(Modules);
	Descriptor.Plugins = GLibConversions::ToNative<FPluginReferenceDescriptor>(Plugins);
	Descriptor.TargetPlatforms = TargetPlatforms;
	Descriptor.EpicSampleNameHash = EpicSampleNameHash;
	Descriptor.PreBuildSteps = PreBuildSteps;
	Descriptor.PostBuildSteps = PostBuildSteps;
	Descriptor.bIsEnterpriseProject = bIsEnterpriseProject;
	Descriptor.bDisableEnginePluginsByDefault = bDisableEnginePluginsByDefault;

	return Descriptor;
}
