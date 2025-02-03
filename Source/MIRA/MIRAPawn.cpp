// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPawn.h"

// Sets default values
AMIRAPawn::AMIRAPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMIRAPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMIRAPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMIRAPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMIRAPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMIRAPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

