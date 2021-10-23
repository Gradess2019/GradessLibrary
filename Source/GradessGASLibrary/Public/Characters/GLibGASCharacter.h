// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Character.h"
#include "GLibGASCharacter.generated.h"

struct FGameplayTagContainer;
class UAbilitySystemComponent;
class UGameplayAbility;

/**
 * @brief A struct that holds information about ability such as a class, level, and an input code
 */
USTRUCT(
	BlueprintType
)
struct FAbilityInfo
{
	GENERATED_BODY()

	/**
	* @brief Ability class
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	TSubclassOf<UGameplayAbility> Class;

	/**
	* @brief Ability level
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	int32 Level;

	/**
	* @brief Input code that trigger the ability
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite
	)
	int32 InputCode;
};

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
	AGLibGASCharacter();

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
	TArray<FAbilityInfo> Abilities;
	
	/**
	 * @brief Character attributes such as health, strength, etc.
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Attributes"
	)
	const class UGLibBaseAttributeSet* Attributes;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
	 * @brief Grants an ability at the given level, with an input code used to pick and choose which ability should be triggered.
	 * @param AbilityClass Ability to create
	 * @param Level level of ability
	 * @param InputCode input code to trigger ability
	 * @return created ability spec handle
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|GASCharacter"
	)
	FGameplayAbilitySpecHandle GrantAbility(
		const TSubclassOf<UGameplayAbility> AbilityClass,
		const int32 Level,
		const int32 InputCode
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
