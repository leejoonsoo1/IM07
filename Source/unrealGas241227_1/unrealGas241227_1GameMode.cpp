// Copyright Epic Games, Inc. All Rights Reserved.

#include "unrealGas241227_1GameMode.h"
#include "unrealGas241227_1Character.h"
#include "UObject/ConstructorHelpers.h"

AunrealGas241227_1GameMode::AunrealGas241227_1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
