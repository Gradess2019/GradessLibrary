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
}

UAbilitySystemComponent* AGLibGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AGLibGASCharacter::GrantAbility(
	const TSubclassOf<UGameplayAbility> AbilityClass,
	int32 Level,
	int32 InputCode,
	FGameplayAbilitySpecHandle& OutAbilitySpecHandle
)
{
	check(AbilitySystemComponent);
	if (!HasAuthority() || !IsValid(AbilityClass)) { return false; }

	const auto NewAbility = AbilityClass.GetDefaultObject();
	if (!IsValid(NewAbility)) { return false; }

	const auto AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level, InputCode);

	OutAbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
	return true;
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
