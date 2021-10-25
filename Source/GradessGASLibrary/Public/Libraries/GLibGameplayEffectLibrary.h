// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GLibGameplayEffectLibrary.generated.h"

/**
 * @brief Library that is providing function for working with gameplay effects
 */
UCLASS()
class GRADESSGASLIBRARY_API UGLibGameplayEffectLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Returns current effect level by effect handle
	 * @param Handle effect handle to use
	 * @return current effect level
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static int32 GetGameplayEffectLevelByHandle(
		const FActiveGameplayEffectHandle& Handle
	);

	/**
	 * @brief Returns current effect level by effect spec
	 * @param Spec effect spec to use
	 * @return current effect level
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static int32 GetGameplayEffectLevelBySpec(const FGameplayEffectSpec& Spec);

	/**
	 * @brief Increases gameplay effect level by delta
	 * @param Handle effect handle to use
	 * @param Delta delta level, usually it is 1
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintAuthorityOnly,
		Category = "GLib|GameplayEffectLibrary"
	)
	static void AddGameplayEffectLevelByHandle(
		UPARAM(ref) FActiveGameplayEffectHandle& Handle,
		const int32 Delta = 1
	);

	/**
	 * @brief Increases level by delta for all gameplay effects
	 * @param AbilitySystem owner ability system component
	 * @param Tags gameplay effect tags
	 * @param Delta delta level, usually it is 1
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintAuthorityOnly,
		Category = "GLib|GameplayEffectLibrary",
		meta = (
			AutoCreateRefTerm = "Tags"
		)
	)
	static void AddGameplayEffectLevelByTags(
		UPARAM(ref) UAbilitySystemComponent*& AbilitySystem,
		const FGameplayTagContainer& Tags,
		const int32 Delta = 1
	);
};
