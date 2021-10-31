// Copyright Gradess. All Rights Reserved.

#pragma once
#if WITH_GAMEPLAY_DEBUGGER
#include "Debugger/DebuggerCategoryRegistrar.h"
#include "GameplayDebugger.h"

template<typename T>
void FDebuggerCategoryRegistrar::RegisterCategory(
	const FName& Name,
	const int32 Key,
	EGameplayDebuggerCategoryState State
)
{
	auto& GameplayDebugger = IGameplayDebugger::Get();
	auto CreationDelegate = IGameplayDebugger::FOnGetCategory::CreateStatic(&T::MakeInstance);

	GameplayDebugger.RegisterCategory(Name, CreationDelegate, State, Key);
	GameplayDebugger.NotifyCategoriesChanged();
}
#endif
