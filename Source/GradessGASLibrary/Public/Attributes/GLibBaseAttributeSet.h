// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GLibBaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * @brief Base attribute set
 */
UCLASS()
class GRADESSGASLIBRARY_API UGLibBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		ReplicatedUsing = OnRep_Health,
		Category = "Attributes"
	)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGLibBaseAttributeSet, Health);

	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		ReplicatedUsing = OnRep_MaxHealth,
		Category = "Attributes"
	)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGLibBaseAttributeSet, MaxHealth);

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
