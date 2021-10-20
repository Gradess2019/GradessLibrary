// Copyright Gradess. All Rights Reserved.


#include "Core/GLibFileToolsLibrary.h"


void UGLibFileToolsLibrary::ValidateConfigFile(const FString& FileName)
{
	const auto ConfigPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir() / FileName);

	auto& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	if (!FileManager.FileExists(*ConfigPath)) { CreateFile(ConfigPath); }
}

void UGLibFileToolsLibrary::CreateFile(const FString& FilePath)
{
	auto& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	const auto File = TUniquePtr<IFileHandle>(FileManager.OpenWrite(*FilePath));
}