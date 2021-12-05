// Copyright Gradess. All Rights Reserved.


#include "Libraries/GLibGameplayEffectLibrary.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

float UGLibGameplayEffectLibrary::GetDurationFromSpec(const FGameplayEffectSpec& Spec)
{
	return Spec.GetDuration();
}

float UGLibGameplayEffectLibrary::GetChanceToApplyToTargetFromSpec(const FGameplayEffectSpec& Spec)
{
	return Spec.GetChanceToApplyToTarget();
}

int32 UGLibGameplayEffectLibrary::GetStackCountFromSpec(const FGameplayEffectSpec& Spec)
{
	return Spec.StackCount;
}

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

void UGLibGameplayEffectLibrary::SetGameplayEffectLevelByHandle(
	FActiveGameplayEffectHandle& Handle,
	const int32 NewLevel
	)
{
	const auto AbilitySystem = Handle.GetOwningAbilitySystemComponent();
	AbilitySystem->SetActiveGameplayEffectLevel(Handle, NewLevel);
}

void UGLibGameplayEffectLibrary::SetGameplayEffectLevelByTags(
	UAbilitySystemComponent*& AbilitySystem,
	const FGameplayTagContainer& Tags,
	const int32 NewLevel
)
{
	auto Effects = AbilitySystem->GetActiveEffectsWithAllTags(Tags);
	for (auto& Effect : Effects)
	{
		SetGameplayEffectLevelByHandle(Effect, NewLevel);
	}
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