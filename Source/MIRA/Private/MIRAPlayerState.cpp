// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPlayerState.h"

AMIRAPlayerState::AMIRAPlayerState()
{
	GameScore = 0;
	CharacterLevel = 1;
}

void AMIRAPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Fixer K"));
	GameScore = 0;
	CharacterLevel = 1;
}
