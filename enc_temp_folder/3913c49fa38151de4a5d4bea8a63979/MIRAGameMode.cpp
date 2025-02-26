// Copyright Epic Games, Inc. All Rights Reserved.

#include "MIRAGameMode.h"
#include "MIRACharacter.h"
#include "MIRAPawn.h"
#include "MIRAPlayerController.h"
#include "MIRAPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AMIRAGameMode::AMIRAGameMode()
{
	// settings for default class
	DefaultPawnClass = AMIRACharacter::StaticClass();
	PlayerControllerClass = AMIRAPlayerController::StaticClass();
	PlayerStateClass = AMIRAPlayerState::StaticClass();
}

void AMIRAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// post login on game mode
	MIRALOG(Warning, TEXT("[PostLogin] called"));
	Super::PostLogin(NewPlayer);

	auto MIRAPlayerState = Cast<AMIRAPlayerState>(NewPlayer->PlayerState);
	MIRACHECK(nullptr != MIRAPlayerState);
	MIRAPlayerState->InitPlayerData();
}
