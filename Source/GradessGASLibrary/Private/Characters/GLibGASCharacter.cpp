// Copyright Gradess. All Rights Reserved.


#include "Characters/GLibGASCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attributes/GLibBaseAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"


AGLibGASCharacter::AGLibGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	Attributes = CreateDefaultSubobject<UGLibBaseAttributeSet>(TEXT("Attributes"));

	CurrentLevel = 1;
}

void AGLibGASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGLibGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	for (const auto Ability : Abilities)
	{
		GrantAbility(Ability, CurrentLevel, INDEX_NONE);
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

	return AbilitySystemComponent->GiveAbility(AbilitySpec);
}

void AGLibGASCharacter::ActivateAbility(const int32 InputCode)
{
	check(AbilitySystemComponent);
	AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
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
