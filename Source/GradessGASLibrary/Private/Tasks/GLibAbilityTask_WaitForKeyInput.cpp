// Copyright Gradess. All Rights Reserved.


#include "Tasks/GLibAbilityTask_WaitForKeyInput.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerInput.h"

UGLibAbilityTask_WaitForKeyInput* UGLibAbilityTask_WaitForKeyInput::WaitForKeyInput(
	UGameplayAbility* OwningAbility,
	const TArray<FKey>& Keys,
	const bool bInTriggerAlreadyPressed
)
{
	const auto NewTask = NewAbilityTask<UGLibAbilityTask_WaitForKeyInput>(OwningAbility);

	for (auto& Key : Keys)
	{
		NewTask->ListenKeys.Add(Key.GetFName(), Key);
	}

	NewTask->bTriggerAlreadyPressed = bInTriggerAlreadyPressed;

	return NewTask;
}

void UGLibAbilityTask_WaitForKeyInput::OnPressed(FKey Key)
{
	if (!ListenKeys.Contains(Key.GetFName())) { return; }
	Pressed.Broadcast(Key);
}

void UGLibAbilityTask_WaitForKeyInput::OnReleased(FKey Key)
{
	if (!ListenKeys.Contains(Key.GetFName())) { return; }
	Released.Broadcast(Key);
}

void UGLibAbilityTask_WaitForKeyInput::Activate()
{
	check(GetOwnerActor() && GetOwnerActor()->IsA(ACharacter::StaticClass()))

	const auto Character = Cast<ACharacter>(GetOwnerActor());
	check(Character->GetController());

	const auto Controller = Cast<AGLibPlayerController>(Character->GetController());
	check(Controller);

	if (bTriggerAlreadyPressed)
	{
		for (const auto& KeyPair : ListenKeys)
		{
			auto& Key = KeyPair.Value;
			if (!Controller->PlayerInput->IsPressed(Key)) { continue; }
			Pressed.Broadcast(Key);
		}
	}

	Controller->OnKeyPressed.AddDynamic(this, &UGLibAbilityTask_WaitForKeyInput::OnPressed);
	Controller->OnKeyReleased.AddDynamic(this, &UGLibAbilityTask_WaitForKeyInput::OnReleased);

	Super::Activate();
}
