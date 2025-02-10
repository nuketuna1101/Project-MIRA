// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperBulletProjectile.h"

// Sets default values
ATrooperBulletProjectile::ATrooperBulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HPBAR(TEXT("/Game/MIRA/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UI_HPBAR.Class);
		HPBar->SetDrawSize(FVector2D(300.0f, 100.0f));
	}
	*/
}

// Called when the game starts or when spawned
void ATrooperBulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrooperBulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

