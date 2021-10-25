// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameFramework/Character.h"
#include "GLibGASCharacter.generated.h"

#pragma region Forward declarations
enum class EGameplayEffectReplicationMode : uint8;
struct FGameplayTagContainer;
class UGameplayEffect;
class UAbilitySystemComponent;
class UGameplayAbility;
class UGLibBaseAttributeSet;
#pragma endregion Forward declarations

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
	* @brief Passive abilities that applies automatically on BeginPlay()
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	/**
	* @brief Passive effects that applies automatically on BeginPlay()
	*/
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Abilities"
	)
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;

	/**
	 * @brief All granted abilities
	 */
	TMap<FString, TArray<FGameplayAbilitySpec>> GrantedAbilities;

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
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Attributes"
	)
	UAttributeSet* Attributes;

	/**
	 * @brief Current entity level
	 */
	UPROPERTY(
		EditAnywhere,
		BlueprintReadOnly,
		Category = "Attributes"
	)
	int32 CurrentLevel;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

protected:
	UFUNCTION(
		BlueprintNativeEvent,
		Category = "Abilities"
	)
	void GrantAbilities();

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "Abilities"
	)
	void ApplyPassiveAbilities();

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "Abilities"
	)
	void ApplyPassiveEffects();

public:
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

	/**
	 * @brief Returns list of active effects that have all of the passed in tags 
	 * @param InTags tags to search
	 * @return list of active effects
	 */
	UFUNCTION(
		BlueprintPure,
		Category = "GLib|GASCharacter",
		meta = (
			AutoCreateRefTerm = "InTags"
		)
	)
	TArray<FActiveGameplayEffectHandle> GetActiveEffectsWithTags(
		UPARAM(DisplayName = "Tags") const FGameplayTagContainer& InTags
	) const;

	/**
	 * @brief Activates ability by tag
	 * @param Tag ability tag
	 * @return true if successful, false otherwise
	 */
	UFUNCTION(
		BlueprintCallable,
		Category = "Abilities",
		meta = (
			AutoCreateRefTerm = "Tag"
		)
	)
	bool ActivateAbility(
		const FGameplayTagContainer& Tag
	);

	/**
	* @brief Return attribute set of specific class
	* @param AttributeClass attributes set to get
	* @return attribute set
	*/
	UFUNCTION(
		BlueprintPure,
		Category = "Attributes",
		meta = (
			DeterminesOutputType = "AttributeClass"
		)
	)
	UAttributeSet* GetAttributes(TSubclassOf<UAttributeSet> AttributeClass) const;

	/**
	* @brief Return attribute set of specific class
	* @tparam T attribute set class
	* @return attribute set
	*/
	template <typename T>
	T* GenericGetAttributes() const;

protected:
	/**
	 * @brief Adds ability to GrantedAbilities map
	 * @param AbilitySpec ability to add
	 */
	UFUNCTION()
	void AddAbilityToContainer(const FGameplayAbilitySpec& AbilitySpec);
};
