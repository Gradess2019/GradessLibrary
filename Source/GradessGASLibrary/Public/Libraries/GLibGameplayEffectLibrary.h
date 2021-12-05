// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GLibGameplayEffectLibrary.generated.h"

class UGameplayEffect;
/**
 * @brief Library that is providing function for working with gameplay effects
 */
UCLASS()
class GRADESSGASLIBRARY_API UGLibGameplayEffectLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static const UGameplayEffect* GetGameplayEffectFromSpec(const FGameplayEffectSpec& Spec);

	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static float GetDurationFromSpec(const FGameplayEffectSpec& Spec);

	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static float GetChanceToApplyToTargetFromSpec(const FGameplayEffectSpec& Spec);

	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GameplayEffectLibrary"
	)
	static int32 GetStackCountFromSpec(const FGameplayEffectSpec& Spec);
	
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
	* @brief Sets gameplay effect level
	* @param Handle effect handle to use
	* @param NewLevel New level of gameplay effect
	*/
	UFUNCTION(
		BlueprintCallable,
		BlueprintAuthorityOnly,
		Category = "GLib|GameplayEffectLibrary"
	)
	static void SetGameplayEffectLevelByHandle(
		UPARAM(ref) FActiveGameplayEffectHandle& Handle,
		const int32 NewLevel = 0
	);

	/**
	 * @brief Sets new effect to all gameplay effects with Tags
	 * @param AbilitySystem Owner ability system component
	 * @param Tags Gameplay effect tags
	 * @param NewLevel New level of gameplay effect
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintAuthorityOnly,
		Category = "GLib|GameplayEffectLibrary",
		meta = (
			AutoCreateRefTerm = "Tags"
		)
	)
	static void SetGameplayEffectLevelByTags(
		UPARAM(ref) UAbilitySystemComponent*& AbilitySystem,
		const FGameplayTagContainer& Tags,
		const int32 NewLevel = 0
	);

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
