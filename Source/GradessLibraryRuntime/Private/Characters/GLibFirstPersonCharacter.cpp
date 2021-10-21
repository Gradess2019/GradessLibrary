// Copyright Gradess. All Rights Reserved.


#include "Characters/GLibFirstPersonCharacter.h"

#include "Components/GLibFirstPersonMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/Controller.h"

const FName AGLibFirstPersonCharacter::MoveForwardName = FName("GLib_MoveForward");
const FName AGLibFirstPersonCharacter::MoveRightName = FName("GLib_MoveRight");
const FName AGLibFirstPersonCharacter::TurnName = FName("GLib_Turn");
const FName AGLibFirstPersonCharacter::LookUpName = FName("GLib_LookUp");
const FName AGLibFirstPersonCharacter::JumpName = FName("GLib_Jump");
const FName AGLibFirstPersonCharacter::CrouchName = FName("GLib_Crouch");
const FName AGLibFirstPersonCharacter::SprintName = FName("GLib_Sprint");

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

	SetupMappings();

	PlayerInputComponent->BindAxis(MoveForwardName, this, &AGLibFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis(MoveRightName, this, &AGLibFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TurnName, this, &AGLibFirstPersonCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(LookUpName, this, &AGLibFirstPersonCharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction(JumpName, IE_Pressed, this, &AGLibFirstPersonCharacter::Jump);
	PlayerInputComponent->BindAction<FBoolDelegate>(CrouchName, IE_Pressed, this, &AGLibFirstPersonCharacter::Crouch, false);
	PlayerInputComponent->BindAction<FBoolDelegate>(CrouchName, IE_Released, this, &AGLibFirstPersonCharacter::UnCrouch, false);
	PlayerInputComponent->BindAction(SprintName, IE_Pressed, this, &AGLibFirstPersonCharacter::OnPressSprint);
	PlayerInputComponent->BindAction(SprintName, IE_Released, this, &AGLibFirstPersonCharacter::OnReleaseSprint);
}

void AGLibFirstPersonCharacter::SetupMappings()
{
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveForwardName, EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveForwardName, EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveForwardName, EKeys::Up, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveForwardName, EKeys::Down, -1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveRightName, EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveRightName, EKeys::Left, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveRightName, EKeys::D, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(MoveRightName, EKeys::Right, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(TurnName, EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(LookUpName, EKeys::MouseY, -1.f));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(JumpName, EKeys::SpaceBar));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(CrouchName, EKeys::LeftControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(CrouchName, EKeys::RightControl));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(SprintName, EKeys::LeftShift));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(SprintName, EKeys::RightShift));
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
