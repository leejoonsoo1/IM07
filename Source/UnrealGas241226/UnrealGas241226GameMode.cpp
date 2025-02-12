// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGas241226GameMode.h"
#include "UnrealGas241226Character.h"
#include "UObject/ConstructorHelpers.h"

AUnrealGas241226GameMode::AUnrealGas241226GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
