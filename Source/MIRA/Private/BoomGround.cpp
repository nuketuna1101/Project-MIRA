// Fill out your copyright notice in the Description page of Project Settings.


#include "BoomGround.h"
#include "TimerManager.h"

// Sets default values
ABoomGround::ABoomGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// component setting
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));

	RootComponent = BoxComp;
	MeshComp->SetupAttachment(BoxComp);
	ParticleComp->SetupAttachment(BoxComp);

	// collision profile setting


	// timer setting
	ElapsedTime = 0.0f;
	UpdateTime = 0.01f;
	BoomTime = 5.0f;

}

// Called when the game starts or when spawned
void ABoomGround::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorldTimerManager().SetTimer(BoomTimerHandle, this, &ABoomGround::BoomTimerUpdate, UpdateTime, true); 

	// dyanmic material instance
	if (MeshComp && MeshComp->GetMaterial(0))
	{
		DynamicMaterial = MeshComp->CreateDynamicMaterialInstance(0);
	}
}

void ABoomGround::BoomTimerUpdate()
{
	ElapsedTime += UpdateTime;

	// if reached boomtime, explode BOOM!
	if (ElapsedTime >= BoomTime)
	{
		GetWorldTimerManager().ClearTimer(BoomTimerHandle);
		BoomExplode();
		return;
	}

	FLinearColor CurrentColor = (
		(ElapsedTime >= BoomTime - 1.0f) ?
		FLinearColor::Red 
		: FMath::Lerp(FLinearColor::White, FLinearColor::Red, (ElapsedTime / BoomTime))
		);

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FName("BoomPointColor"), CurrentColor);
	}
}

void ABoomGround::BoomExplode()
{
	MIRALOG(Warning, TEXT("BOOM!"));
	if (ParticleComp)
	{
		ParticleComp->Activate(true);
	}
}