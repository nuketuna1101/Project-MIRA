// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABlade.h"

// Sets default values
AMIRABlade::AMIRABlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		SK_MIRABLADE(TEXT("/Game/MIRA/Characters/Meshes/SM_MPlayerBladeGlow.SM_MPlayerBladeGlow"));

	if (SK_MIRABLADE.Succeeded())
	{
		Weapon->SetStaticMesh(SK_MIRABLADE.Object);
	}

	// collision setting
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AMIRABlade::BeginPlay()
{
	Super::BeginPlay();
	
}

