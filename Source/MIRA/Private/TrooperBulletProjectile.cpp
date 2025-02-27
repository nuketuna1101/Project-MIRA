// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperBulletProjectile.h"
#include "Components/ArrowComponent.h"

// Sets default values
ATrooperBulletProjectile::ATrooperBulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// component setting
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> 
		FX_TROOPERBULLET(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_Trail.P_Wraith_Primary_Trail"));
	if (FX_TROOPERBULLET.Succeeded())
	{
		ParticleComp->SetTemplate(FX_TROOPERBULLET.Object);
	}

	RootComponent = MeshComp;
	BoxComp->SetupAttachment(MeshComp);
	ParticleComp->SetupAttachment(MeshComp);
	ArrowComp->SetupAttachment(ParticleComp);

	// collision profile setting
	BoxComp->SetCollisionProfileName(TEXT("EnemyProjectile")); 

	// projectilemovement comp setting
	ProjectileMovement->InitialSpeed = 400.0f;
	ProjectileMovement->MaxSpeed = 400.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 800.0f;
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bInterpRotation = true;

}

// Called when the game starts or when spawned
void ATrooperBulletProjectile::BeginPlay()
{
	Super::BeginPlay();

	// set lifecount
	CurLifeCount = LifeSpan;

}

// Called every frame
void ATrooperBulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurLifeCount > 0.0f)
	{
		// if available, check collision with player character

		// Ray cast w. line trace 
		FHitResult hitResult;
		FCollisionObjectQueryParams objCollisionQueryParams;
		FVector CurLoc = GetActorLocation();
		FVector NextLoc = CurLoc + GetActorForwardVector() * ProjectileMovement->MaxSpeed * DeltaTime;
		// check if first collided
		if (GetWorld()->LineTraceSingleByObjectType(hitResult, CurLoc, NextLoc, objCollisionQueryParams))
		{
			auto TheActor = hitResult.GetActor();
			if (hitResult.GetActor() != nullptr)
			{
				MIRALOG(Warning, TEXT("[Projectile] hit called"));
				// remove it
				PrimaryActorTick.bCanEverTick = false;
				Destroy();
			}
		}

		// Reduce time
		CurLifeCount -= DeltaTime;
	}
	else
	{
		// automatically returned if out of time
		MIRALOG(Warning, TEXT(" projectile gone"));
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}
}

