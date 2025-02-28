// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomGround.h"

// Sets default values
ABoomGround::ABoomGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoomGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoomGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

