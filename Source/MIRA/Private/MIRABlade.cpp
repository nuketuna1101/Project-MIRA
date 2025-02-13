// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABlade.h"

// Sets default values
AMIRABlade::AMIRABlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MIRABLADE
	(TEXT(""));

	if (SK_MIRABLADE.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_MIRABLADE.Object);
	}

	// collision setting
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AMIRABlade::BeginPlay()
{
	Super::BeginPlay();
	
}

