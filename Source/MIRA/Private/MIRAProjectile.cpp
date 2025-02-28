// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAProjectile.h"
#include "MIRAPlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMIRAProjectile::AMIRAProjectile()
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
	BoxComp->SetBoxExtent(FVector(75.0f, 15.0f, 15.0f));

	// particle scaling
	ParticleComp->SetRelativeScale3D(FVector(0.5f, 1.0f, 1.0f));

	// projectilemovement comp setting
	ProjectileMovement->InitialSpeed = 400.0f;
	ProjectileMovement->MaxSpeed = 400.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bInterpMovement = true;
	ProjectileMovement->bInterpRotation = true;

	// default homing is false
	bIsMIRAHoming = false;
}

// Called when the game starts or when spawned
void AMIRAProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// set lifecount
	CurLifeCount = LifeSpan;

	if (bIsMIRAHoming)
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingAccelerationMagnitude = 800.0f;
		ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
	}
	else
	{
		ProjectileMovement->bIsHomingProjectile = false;
	}
}

// Called every frame
void AMIRAProjectile::Tick(float DeltaTime)
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
			if (nullptr != hitResult.GetActor())
			{
				// 플레이어 캐릭터인지 확인

				//AMIRAPlayerCharacter* HitPlayer = Cast<AMIRAPlayerCharacter>(TheActor);
				//if (nullptr != HitPlayer)
				if (TargetClass.GetDefaultObject()->GetClass()->IsChildOf(TheActor->GetClass()))
				{
					// player take damage
					FDamageEvent DamageEvent;
					//HitPlayer->TakeDamage(20.0f, DamageEvent, nullptr, this);
					TheActor->TakeDamage(20.0f, DamageEvent, nullptr, this);

					// fx for impact location
					FVector ImpactLocation = hitResult.ImpactPoint;
					BulletOnHitBP.Broadcast(ImpactLocation);

					MIRALOG(Warning, TEXT("[TBP] hit called"));
					PrimaryActorTick.bCanEverTick = false;
					Destroy();
				}
			}
		}
		// Reduce time
		CurLifeCount -= DeltaTime;
	}
	else
	{
		// automatically returned if out of time
		MIRALOG(Warning, TEXT("[TBP] projectile gone"));
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}
}

