// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPoolable.h"

AMIRAPoolable::AMIRAPoolable()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMIRAPoolable::BeginPlay()
{
	Super::BeginPlay();
}

void AMIRAPoolable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMIRAPoolable::Activate(bool bActivationFlag)
{
	bIsActive = bActivationFlag;
	SetActorEnableCollision(bActivationFlag);
	SetActorTickEnabled(bActivationFlag);
	SetActorHiddenInGame(!bActivationFlag);
}