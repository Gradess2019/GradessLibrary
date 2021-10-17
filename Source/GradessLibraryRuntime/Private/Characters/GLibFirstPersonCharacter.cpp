// Copyright Gradess. All Rights Reserved.


#include "Characters/GLibFirstPersonCharacter.h"

#include "Components/GLibFirstPersonMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"

AGLibFirstPersonCharacter::AGLibFirstPersonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UGLibFirstPersonMovementComponent>(CharacterMovementComponentName)
	)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	check(GetCharacterMovement()->Implements<USprintComponent>())
}

void AGLibFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveForward", EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveForward", EKeys::Up, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveForward", EKeys::Down, -1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveRight", EKeys::Left, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveRight", EKeys::D, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_MoveRight", EKeys::Right, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_Turn", EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GLib_LookUp", EKeys::MouseY, -1.f));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GLib_Jump", EKeys::SpaceBar));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GLib_Crouch", EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GLib_Crouch", EKeys::RightControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GLib_Sprint", EKeys::LeftShift));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("GLib_Sprint", EKeys::RightShift));

	PlayerInputComponent->BindAxis("GLib_MoveForward", this, &AGLibFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("GLib_MoveRight", this, &AGLibFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("GLib_Turn", this, &AGLibFirstPersonCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("GLib_LookUp", this, &AGLibFirstPersonCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("GLib_Jump", IE_Pressed, this, &AGLibFirstPersonCharacter::Jump);
	PlayerInputComponent->BindAction<FBoolDelegate>("GLib_Crouch", IE_Pressed, this, &AGLibFirstPersonCharacter::Crouch,
	                                                false);
	PlayerInputComponent->BindAction<FBoolDelegate>("GLib_Crouch", IE_Released, this,
	                                                &AGLibFirstPersonCharacter::UnCrouch, false);
	PlayerInputComponent->BindAction("GLib_Sprint", IE_Pressed, this, &AGLibFirstPersonCharacter::OnPressSprint);
	PlayerInputComponent->BindAction("GLib_Sprint", IE_Released, this, &AGLibFirstPersonCharacter::OnReleaseSprint);
}

void AGLibFirstPersonCharacter::MoveForward_Implementation(const float InputValue)
{
	if (InputValue != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			AddMovementInput(GetActorForwardVector(), InputValue);
		}
	}
}

void AGLibFirstPersonCharacter::MoveRight_Implementation(const float InputValue)
{
	if (InputValue != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			AddMovementInput(GetActorRightVector(), InputValue);
		}
	}
}

void AGLibFirstPersonCharacter::OnPressSprint_Implementation()
{
	ISprintComponent::Execute_Sprint(GetCharacterMovement());
}

void AGLibFirstPersonCharacter::OnReleaseSprint_Implementation()
{
	ISprintComponent::Execute_UnSprint(GetCharacterMovement());
}
