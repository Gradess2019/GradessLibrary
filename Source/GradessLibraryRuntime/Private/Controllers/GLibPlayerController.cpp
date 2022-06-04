// Copyright Gradess. All Rights Reserved.


#include "Controllers/GLibPlayerController.h"

#include "GameFramework/PlayerInput.h"

bool AGLibPlayerController::InputKey(FKey Key, EInputEvent Event, float AmountDepressed, bool bGamepad)
{
	const auto KeyParams = FInputKeyParams(Key, Event, static_cast<double>(AmountDepressed), bGamepad);
	const auto Result = Super::InputKey(KeyParams);
	if (Event == IE_Pressed)
	{
		OnKeyPressed.Broadcast(Key);
	}
	else if (Event == IE_Released)
	{
		OnKeyReleased.Broadcast(Key);
	}
	
	return Result;
}
