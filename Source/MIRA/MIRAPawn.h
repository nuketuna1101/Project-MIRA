// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Pawn.h"
#include "MIRAPawn.generated.h"

UCLASS()
class MIRA_API AMIRAPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMIRAPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//
	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;
};
