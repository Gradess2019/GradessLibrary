// Copyright Gradess. All Rights Reserved.


#include "Actions/GLibAsyncActionAttributeChanged.h"

#include "AbilitySystemComponent.h"

UGLibAsyncActionAttributeChanged* UGLibAsyncActionAttributeChanged::ListenForAttributeChange(
	UAbilitySystemComponent* InAbilitySystemComponent,
	const FGameplayAttribute Attribute
)
{
	const auto ListenForAttributeChange = NewObject<UGLibAsyncActionAttributeChanged>();

	if (!IsValid(InAbilitySystemComponent) || !Attribute.IsValid())
	{
		ListenForAttributeChange->RemoveFromRoot();
		return nullptr;
	}

	ListenForAttributeChange->AbilitySystemComponent = InAbilitySystemComponent;
	ListenForAttributeChange->AttributeToListen = Attribute;

	BindToDelegate(InAbilitySystemComponent, ListenForAttributeChange, Attribute);
	return ListenForAttributeChange;
}

UGLibAsyncActionAttributeChanged* UGLibAsyncActionAttributeChanged::ListenForAttributesChange(
	UAbilitySystemComponent* InAbilitySystemComponent,
	const TArray<FGameplayAttribute>& Attributes
)
{
	const auto ListenForAttributeChange = NewObject<UGLibAsyncActionAttributeChanged>();
	if (!IsValid(InAbilitySystemComponent) || Attributes.Num() < 1)
	{
		ListenForAttributeChange->RemoveFromRoot();
		return nullptr;
	}

	ListenForAttributeChange->AbilitySystemComponent = InAbilitySystemComponent;
	ListenForAttributeChange->AttributesToListen = Attributes;


	for (FGameplayAttribute Attribute : Attributes)
	{
		BindToDelegate(InAbilitySystemComponent, ListenForAttributeChange, Attribute);
	}

	return ListenForAttributeChange;
}

void UGLibAsyncActionAttributeChanged::BindToDelegate(
	UAbilitySystemComponent* InAbilitySystemComponent,
	UGLibAsyncActionAttributeChanged* ListenForAttributeChange,
	const FGameplayAttribute& Attribute)
{
	InAbilitySystemComponent
		->GetGameplayAttributeValueChangeDelegate(Attribute)
		.AddUObject(ListenForAttributeChange, &UGLibAsyncActionAttributeChanged::AttributeChanged);
}

void UGLibAsyncActionAttributeChanged::EndTask()
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeToListen).RemoveAll(this);

		for (const auto& Attribute : AttributesToListen)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	RemoveFromRoot();
	SetReadyToDestroy();
	MarkPendingKill();
}

void UGLibAsyncActionAttributeChanged::BeginDestroy()
{
	EndTask();

	Super::BeginDestroy();
}

void UGLibAsyncActionAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	// BUG: Why does delegate fire twice per change?
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
