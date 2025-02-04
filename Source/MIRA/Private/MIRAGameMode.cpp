// Copyright Epic Games, Inc. All Rights Reserved.

#include "MIRAGameMode.h"
#include "MIRACharacter.h"
#include "MIRAPawn.h"
#include "MIRAPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMIRAGameMode::AMIRAGameMode()
{
	/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/

	// settings for default class
	DefaultPawnClass = AMIRACharacter::StaticClass();
	PlayerControllerClass = AMIRAPlayerController::StaticClass();
	
}

void AMIRAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// 
	MIRALOG(Warning, TEXT("[PostLogin] called"));
	Super::PostLogin(NewPlayer);
}
