// Copyright Gradess. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GLibFirstPersonCharacter.generated.h"

#pragma region DelegateDeclarations
DECLARE_DELEGATE_OneParam(FBoolDelegate, bool);
#pragma endregion DelegateDeclarations

UCLASS(
	Blueprintable,
	BlueprintType,
	ClassGroup = (
		GLib
	)
)
class GRADESSLIBRARYRUNTIME_API AGLibFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGLibFirstPersonCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetupMappings();

protected:
	UFUNCTION(
		BlueprintNativeEvent,
		Category = "GLib|Characters"
	)
	void MoveForward(const float InputValue);

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "GLib|Characters"
	)
	void MoveRight(const float InputValue);

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "GLib|Characters"
	)
	void OnPressSprint();

	UFUNCTION(
		BlueprintNativeEvent,
		Category = "GLib|Characters"
	)
	void OnReleaseSprint();

	static const FName MoveForwardName;
	static const FName MoveRightName;
	static const FName TurnName;
	static const FName LookUpName;
	static const FName JumpName;
	static const FName CrouchName;
	static const FName SprintName;
	
};
