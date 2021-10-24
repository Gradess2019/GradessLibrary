// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Character.h"
#include "GLibGASCharacter.generated.h"

enum class EGameplayEffectReplicationMode : uint8;
class UGameplayEffect;
struct FGameplayTagContainer;
class UAbilitySystemComponent;
class UGameplayAbility;
class UGLibBaseAttributeSet;

/**
 * @brief Base GAS character
 */
UCLASS(
	BlueprintType,
	Blueprintable,
	ClassGroup = (
		GLib
	)
)
class GRADESSGASLIBRARY_API AGLibGASCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGLibGASCharacter(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * @brief Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities.
	 */
	UPROPERTY(
		VisibleDefaultsOnly,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	UAbilitySystemComponent* AbilitySystemComponent;

protected:
	/**
	 * @brief Available abilities
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	/**
	* @brief Available abilities
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

	/**
	 * @brief Ability system component replication mode
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	EGameplayEffectReplicationMode ReplicationMode;

	/**
	 * @brief Character attributes such as health, strength, etc.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Attributes"
	)
	const UAttributeSet* Attributes;

	/**
	 * @brief Current entity level
	 */
	UPROPERTY(
		BlueprintReadOnly,
		Category = "Attributes"
	)
	int32 CurrentLevel;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	 * @brief Grants an ability at the given level, with an input code used to pick and choose which ability should be triggered.
	 * @param AbilityClass ability to create
	 * @param Level level of ability. If equals -1 then uses character level
	 * @param InputCode input code to trigger ability
	 * @return created ability spec handle
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|GASCharacter"
	)
	FGameplayAbilitySpecHandle GrantAbility(
		const TSubclassOf<UGameplayAbility> AbilityClass,
		int32 Level = -1,
		const int32 InputCode = -1
	);

	/**
	 * @brief Activates an ability with a matching input code
	 * @param InputCode input code to trigger ability
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|GASCharacter"
	)
	void ActivateAbility(const int32 InputCode);

	/**
	 * @brief Cancel all abilities with the specified tags. Will not cancel the Ignore instance
	 * @param WithTags cancel abilities with tags
	 * @param WithoutTags cancel abilities without tags
	 * @param Ignore ability to ignore
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|GASCharacter",
		meta = (
			AutoCreateRefTerm = "WithTags, WithoutTags"
		)
	)
	void CancelAbilityWithTags(
		const FGameplayTagContainer& WithTags,
		const FGameplayTagContainer& WithoutTags,
		UGameplayAbility* Ignore = nullptr
	);
};
