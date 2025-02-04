// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPlayerController.h"

void AMIRAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMIRAPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMIRAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// exclude ui and only send input to game
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
