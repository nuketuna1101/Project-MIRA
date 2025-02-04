// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/PlayerController.h"
#include "MIRAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// components inits after instance created
	virtual void PostInitializeComponents() override;
	// use pawn by possessing
	virtual void OnPossess(APawn* aPawn) override;


protected:
	virtual void BeginPlay() override;

};
