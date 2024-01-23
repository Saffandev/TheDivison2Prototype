// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheDivison2GameMode.h"
#include "TheDivison2Character.h"
#include "UObject/ConstructorHelpers.h"

ATheDivison2GameMode::ATheDivison2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
