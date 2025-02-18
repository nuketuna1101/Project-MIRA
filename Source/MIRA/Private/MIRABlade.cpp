// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABlade.h"

// Sets default values
AMIRABlade::AMIRABlade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX COLLIDER"));
	RootComponent = Weapon;
	BoxCollider->SetupAttachment(Weapon);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SK_MIRABLADE(TEXT("/Game/MIRA/Characters/Meshes/SM_MPlayerBladeGlow.SM_MPlayerBladeGlow"));
	if (SK_MIRABLADE.Succeeded())
	{
		Weapon->SetStaticMesh(SK_MIRABLADE.Object);
	}

	// collision setting
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	//BoxCollider->SetCollisionProfileName(TEXT("Weapon"));
	//BoxCollider->SetGenerateOverlapEvents(true);

	// Box Collider setting
	BoxCollider->SetBoxExtent(FVector(20.0f, 3.0f, 3.0f));
}

//void AMIRABlade::OnBeginOverlap(UPrimitiveComponent* OverlapeedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	AMIRAEnemyBaseCharacter* TargetEnemy = Cast<AMIRAEnemyBaseCharacter>(OtherActor);
//	if (TargetEnemy)
//	{
//		MIRALOG(Warning, TEXT("[OnBeginOverlap]"));
//		OverlappedEnemies.Add(TargetEnemy);
//	}
//}
//
//void AMIRABlade::OnEndOverlap(UPrimitiveComponent* OverlapeedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
//{
//	AMIRAEnemyBaseCharacter* TargetEnemy = Cast<AMIRAEnemyBaseCharacter>(OtherActor);
//	if (TargetEnemy)
//	{
//		MIRALOG(Warning, TEXT("[OnEndOverlap]"));
//		OverlappedEnemies.Remove(TargetEnemy);
//	}
//}

// Called when the game starts or when spawned
void AMIRABlade::BeginPlay()
{
	Super::BeginPlay();

}