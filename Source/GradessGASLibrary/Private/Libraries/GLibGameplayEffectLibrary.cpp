// Copyright Gradess. All Rights Reserved.


#include "Libraries/GLibGameplayEffectLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

int32 UGLibGameplayEffectLibrary::GetGameplayEffectLevelByHandle(const FActiveGameplayEffectHandle& Handle)
{
	const auto AbilitySystem = Handle.GetOwningAbilitySystemComponent();
	check(AbilitySystem)

	const auto Effect = AbilitySystem->GetActiveGameplayEffect(Handle);
	return Effect->Spec.GetLevel();
}

int32 UGLibGameplayEffectLibrary::GetGameplayEffectLevelBySpec(const FGameplayEffectSpec& Spec)
{
	return Spec.GetLevel();
}

void UGLibGameplayEffectLibrary::AddGameplayEffectLevelByHandle(
	FActiveGameplayEffectHandle& Handle,
	const int32 Delta
)
{
	const auto AbilitySystem = Handle.GetOwningAbilitySystemComponent();
	const auto CurrentLevel = GetGameplayEffectLevelByHandle(Handle);
	AbilitySystem->SetActiveGameplayEffectLevel(Handle, CurrentLevel + Delta);
}

void UGLibGameplayEffectLibrary::AddGameplayEffectLevelByTags(
	UAbilitySystemComponent*& AbilitySystem,
	const FGameplayTagContainer& Tags,
	const int32 Delta
)
{
	auto Effects = AbilitySystem->GetActiveEffectsWithAllTags(Tags);
	for (auto& Effect : Effects)
	{
		AddGameplayEffectLevelByHandle(Effect, Delta);
	}
}
