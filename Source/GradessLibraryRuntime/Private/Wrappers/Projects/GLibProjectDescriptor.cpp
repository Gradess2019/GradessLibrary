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
	TargetPlatforms = InDescriptor.TargetPlatforms;
	EpicSampleNameHash = InDescriptor.EpicSampleNameHash;
	PreBuildSteps = InDescriptor.PreBuildSteps;
	PostBuildSteps = InDescriptor.PostBuildSteps;
	bIsEnterpriseProject = InDescriptor.bIsEnterpriseProject;
	bDisableEnginePluginsByDefault = InDescriptor.bDisableEnginePluginsByDefault;
	AdditionalPluginDirectories = InDescriptor.GetAdditionalPluginDirectories();
	AdditionalRootDirectories = InDescriptor.GetAdditionalRootDirectories();

	GLibConversions::ToBlueprintable<FGLibModuleDescriptor>(InDescriptor.Modules, Modules);
	GLibConversions::ToBlueprintable<FGLibPluginReferenceDescriptor>(InDescriptor.Plugins, Plugins);
}

FGLibProjectDescriptor::operator FProjectDescriptor() const
{
	FProjectDescriptor Descriptor;
	Descriptor.FileVersion = GLibConversions::ToNative<EProjectDescriptorVersion::Type>(FileVersion);
	Descriptor.EngineAssociation = EngineAssociation;
	Descriptor.Category = Category;
	Descriptor.Description = Description;
	Descriptor.TargetPlatforms = TargetPlatforms;
	Descriptor.EpicSampleNameHash = EpicSampleNameHash;
	Descriptor.PreBuildSteps = PreBuildSteps;
	Descriptor.PostBuildSteps = PostBuildSteps;
	Descriptor.bIsEnterpriseProject = bIsEnterpriseProject;
	Descriptor.bDisableEnginePluginsByDefault = bDisableEnginePluginsByDefault;

	GLibConversions::ToNative<FModuleDescriptor>(Modules, Descriptor.Modules);
	GLibConversions::ToNative<FPluginReferenceDescriptor>(Plugins, Descriptor.Plugins);

	return Descriptor;
}
