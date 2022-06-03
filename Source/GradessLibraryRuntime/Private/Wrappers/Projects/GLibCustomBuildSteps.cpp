// Fill out your copyright notice in the Description page of Project Settings.


#include "Wrappers/Projects/GLibCustomBuildSteps.h"

FGLibCustomBuildSteps::FGLibCustomBuildSteps()
{}

FGLibCustomBuildSteps::FGLibCustomBuildSteps(const FCustomBuildSteps& Native)
{
	for (auto& Command : Native.HostPlatformToCommands)
	{
		HostPlatformToCommands.Add(Command.Key, Command.Value);
	}
}

FGLibCustomBuildSteps::operator FCustomBuildSteps() const
{
	FCustomBuildSteps Result;
	for (auto& Command : HostPlatformToCommands)
	{
		Result.HostPlatformToCommands.Add(Command.Key, Command.Value);
	}

	return Result;
}

