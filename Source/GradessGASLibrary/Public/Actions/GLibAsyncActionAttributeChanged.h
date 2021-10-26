// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GLibAsyncActionAttributeChanged.generated.h"

#pragma region Forward declarations
struct FOnAttributeChangeData;
class UAbilitySystemComponent;
#pragma endregion Forward declarations

#pragma region Delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FGLibOnAttributeChanged,
	FGameplayAttribute, Attribute,
	float, NewValue,
	float, OldValue
);
#pragma endregion Delegate declarations

/**
 * @brief Async action for listening attributes change events
 */
UCLASS()
class GRADESSGASLIBRARY_API UGLibAsyncActionAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Fires when attribute changed
	 */
	UPROPERTY(
		BlueprintAssignable
	)
	FGLibOnAttributeChanged OnAttributeChanged;

	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|AsyncTaskAttributeChanged",
		meta = (
			BlueprintInternalUseOnly = "true",
			AutoCreateRefTerm = "Attribute"
		)
	)
	static UGLibAsyncActionAttributeChanged* ListenForAttributeChange(
		UPARAM(DisplayName = "AbilitySystemComponent") UAbilitySystemComponent* InAbilitySystemComponent,
		const FGameplayAttribute Attribute
	);

	UFUNCTION(
		BlueprintCallable,
		Category = "GLib|AsyncTaskAttributeChanged",
		meta = (
			BlueprintInternalUseOnly = "true"
		)
	)
	static UGLibAsyncActionAttributeChanged* ListenForAttributesChange(
		UPARAM(DisplayName = "AbilitySystemComponent") UAbilitySystemComponent* InAbilitySystemComponent,
		const TArray<FGameplayAttribute>& Attributes
	);

	/**
	 * @brief Clears all delegates of the action, attached to ability system
	 */
	UFUNCTION()
	void EndTask();
	
	// BUG: Garbage collection runs periodically, so our delegate fires till parent is not garbage collected
	virtual void BeginDestroy() override;

protected:
	/**
	 * @brief Ability system component to attach delegates to
	 */
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	/**
	 * @brief Single attribute to listen
	 */
	FGameplayAttribute AttributeToListen;
	
	/**
	 * @brief Group of attributes to listen 
	 */
	TArray<FGameplayAttribute> AttributesToListen;

	/**
	 * @brief Bind delegate to attribute change delegate
	 * @param InAbilitySystemComponent ability system component to attach delegates to
	 * @param ListenForAttributeChange blueprint action node
	 * @param Attribute attribute to listen
	 */
	UFUNCTION()
	static void BindToDelegate(
		UPARAM(DisplayName = "AbilitySystemComponent") UAbilitySystemComponent* InAbilitySystemComponent,
		UGLibAsyncActionAttributeChanged* ListenForAttributeChange,
		const FGameplayAttribute& Attribute
	);

	void AttributeChanged(const FOnAttributeChangeData& Data);
};
