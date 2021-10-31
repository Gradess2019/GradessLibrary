// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if WITH_GAMEPLAY_DEBUGGER
#include "DebuggerCategoryRegistrar.h"
#include "GameplayDebugger.h"


/**
 * @brief Helper class for registering debugger categories
 */
class GRADESSLIBRARYRUNTIME_API FDebuggerCategoryRegistrar
{
public:
	/**
	 * @brief Registers new debugger category that can be activated in menu (to open menu press apostrophe "'") 
	 * @tparam T Debugger category class
	 * @param Name Category Name
	 * @param Key Category hotkey
	 * @param State Where category should be active
	 */
	template<typename T>
	static void RegisterCategory(
		const FName& Name,
		const int32 Key,
		EGameplayDebuggerCategoryState State = EGameplayDebuggerCategoryState::EnabledInGameAndSimulate
	);
};
#endif