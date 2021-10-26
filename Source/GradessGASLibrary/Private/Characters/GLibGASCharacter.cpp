// Copyright Gradess. All Rights Reserved.


#include "Characters/GLibGASCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attributes/GLibBaseAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbilitySpec.h"

AGLibGASCharacter::AGLibGASCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);

	ReplicationMode = EGameplayEffectReplicationMode::Minimal;

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UAbilitySystemComponent>(
		this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ReplicationMode);

	Attributes = ObjectInitializer.CreateDefaultSubobject<UGLibBaseAttributeSet>(this, TEXT("AttributeSet"));

	CurrentLevel = 1;
}

void AGLibGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAttributeSet* AGLibGASCharacter::GetAttributes(TSubclassOf<UAttributeSet> AttributeClass) const
{
	return Attributes;
}

void AGLibGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	GrantAbilities();
	ApplyPassiveAbilities();
	ApplyPassiveEffects();
}

void AGLibGASCharacter::GrantAbilities_Implementation()
{
	for (const auto& AbilityClass : Abilities)
	{
		GrantAbility(AbilityClass, CurrentLevel, INDEX_NONE);
	}
}

void AGLibGASCharacter::ApplyPassiveAbilities_Implementation()
{
	for (const auto& AbilityClass : PassiveAbilities)
	{
		GrantAbility(AbilityClass, CurrentLevel, INDEX_NONE);
		AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass);
	}
}

void AGLibGASCharacter::ApplyPassiveEffects_Implementation()
{
	for (const auto& EffectClass : PassiveEffects)
	{
		auto EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		auto EffectHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, CurrentLevel, EffectContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectHandle.Data.Get());
	}
}

UAbilitySystemComponent* AGLibGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FGameplayAbilitySpecHandle AGLibGASCharacter::GrantAbility(
	const TSubclassOf<UGameplayAbility> AbilityClass,
	int32 Level,
	const int32 InputCode
)
{
	check(AbilitySystemComponent);
	if (!HasAuthority() || !IsValid(AbilityClass)) { return FGameplayAbilitySpecHandle(); }

	const auto NewAbility = AbilityClass.GetDefaultObject();
	if (!IsValid(NewAbility)) { return FGameplayAbilitySpecHandle(); }

	Level = Level == INDEX_NONE ? CurrentLevel : Level;
	
	const auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level, InputCode);

	AddAbilityToContainer(AbilitySpec);
	
	return AbilitySystemComponent->GiveAbility(AbilitySpec);
}

void AGLibGASCharacter::AddAbilityToContainer(const FGameplayAbilitySpec& AbilitySpec)
{
	const auto& StringTag = AbilitySpec.Ability->AbilityTags.ToStringSimple();
	if (!GrantedAbilities.Contains(StringTag))
	{
		GrantedAbilities.Add(StringTag, TArray<FGameplayAbilitySpec>());
	}

	// BUG: Possible to add more than one spec with same ability!
	GrantedAbilities[StringTag].Add(AbilitySpec);
}

void AGLibGASCharacter::CancelAbilityWithTags(
	const FGameplayTagContainer& WithTags,
	const FGameplayTagContainer& WithoutTags,
	UGameplayAbility* Ignore
)
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->CancelAbilities(&WithTags, &WithoutTags, Ignore);
}

TArray<FActiveGameplayEffectHandle> AGLibGASCharacter::GetActiveEffectsWithTags(
	const FGameplayTagContainer& InTags) const
{
	return AbilitySystemComponent->GetActiveEffectsWithAllTags(InTags);
}

bool AGLibGASCharacter::ActivateAbility(const FGameplayTagContainer& Tag)
{
	const auto StringTag = Tag.ToStringSimple();
	if (!GrantedAbilities.Contains(StringTag)) { return false; }
	
	auto bSuccessful = true;
	auto& GrantedAbilitySpecs = GrantedAbilities[StringTag];
	for (const auto& Ability : GrantedAbilitySpecs)
	{
		bSuccessful = bSuccessful && AbilitySystemComponent->TryActivateAbility(Ability.Handle);
	}

	return bSuccessful;
}
