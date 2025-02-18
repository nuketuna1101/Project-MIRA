// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABlade.h"
#include "Engine/DamageEvents.h"

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
	BoxCollider->SetCollisionProfileName(TEXT("MIRABlade"));

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMIRABlade::OnBeginOverlap);
	// Box Collider setting
	BoxCollider->SetBoxExtent(FVector(20.0f, 3.0f, 3.0f));
}

// Called when the game starts or when spawned
void AMIRABlade::BeginPlay()
{
	Super::BeginPlay();

}

void AMIRABlade::OnBeginOverlap(UPrimitiveComponent* OverlapeedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMIRAEnemyBaseCharacter* TargetEnemy = Cast<AMIRAEnemyBaseCharacter>(OtherActor);
	if (TargetEnemy)
	{
		if (!OverlappedEnemies.Contains(TargetEnemy))
		{
			OverlappedEnemies.Add(TargetEnemy);
		}
	}
}

void AMIRABlade::ApplyBladeAttack()
{
	for (AMIRAEnemyBaseCharacter* Enemy : OverlappedEnemies)
	{
		if (Enemy && Enemy != GetOwner())
		{
			// take damage
			MIRALOG(Warning, TEXT("[ApplyBladeAttack]"));

			FDamageEvent DamageEvent;
			Enemy->TakeDamage(20.0f, DamageEvent, nullptr, this);
			//OnHitBP.Broadcast(HitResult.ImpactPoint);
		}
	}
	//
	OverlappedEnemies.Empty();
}

TArray<AMIRAEnemyBaseCharacter*> AMIRABlade::GetOverlappedEnemies()
{
	return OverlappedEnemies;
}
