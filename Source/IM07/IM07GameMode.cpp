// Copyright Epic Games, Inc. All Rights Reserved.

#include "IM07GameMode.h"
#include "IM07Character.h"
#include "UObject/ConstructorHelpers.h"

AIM07GameMode::AIM07GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
