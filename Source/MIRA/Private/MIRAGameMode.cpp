// Copyright Epic Games, Inc. All Rights Reserved.

#include "MIRAGameMode.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAPawn.h"
#include "MIRAPlayerController.h"
#include "MIRAPlayerState.h"
#include "MIRAGameState.h"
#include "UObject/ConstructorHelpers.h"

AMIRAGameMode::AMIRAGameMode()
{
	// settings for default class
	DefaultPawnClass = AMIRAPlayerCharacter::StaticClass();
	PlayerControllerClass = AMIRAPlayerController::StaticClass();
	PlayerStateClass = AMIRAPlayerState::StaticClass();
	GameStateClass = AMIRAGameState::StaticClass();
}

void AMIRAGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MIRAGameState = Cast<AMIRAGameState>(GameState);
}

void AMIRAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// post login on game mode
	Super::PostLogin(NewPlayer);

	auto MIRAPlayerState = Cast<AMIRAPlayerState>(NewPlayer->PlayerState);
	MIRACHECK(nullptr != MIRAPlayerState);
	MIRAPlayerState->InitPlayerData();
}
