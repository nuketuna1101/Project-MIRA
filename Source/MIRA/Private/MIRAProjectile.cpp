// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAProjectile.h"
#include "MIRAPlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"
#include "CollisionQueryParams.h"

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
	if (!IsValid(this)) 	return;
	
	Super::Tick(DeltaTime);

	if (CurLifeCount > 0.0f)
	{
		// if available, check collision with player character

		// Ray cast w. line trace 
		FHitResult hitResult;
		FCollisionObjectQueryParams objCollisionQueryParams;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		FVector CurLoc = GetActorLocation();
		FVector NextLoc = CurLoc + GetActorForwardVector() * ProjectileMovement->MaxSpeed * DeltaTime;
		// check if first collided
		if (GetWorld()->LineTraceSingleByObjectType(hitResult, CurLoc, NextLoc, objCollisionQueryParams, CollisionQueryParams))
		{
			auto HitActor = hitResult.GetActor();
			if (nullptr != HitActor)
			{
				if (nullptr == TargetClass) return;
				// exception for owner charachter
				if (this->Owner->IsValidLowLevel() && HitActor == this->Owner.Get()) return;
				// exception for projectile itself
				if (nullptr != Cast<AMIRAProjectile>(HitActor)) return;
				FVector ImpactLocation = hitResult.ImpactPoint;
				//if (TargetClass.GetDefaultObject()->GetClass()->IsChildOf(HitActor->GetClass()))
				if (HitActor->GetClass()->IsChildOf(TargetClass.GetDefaultObject()->GetClass()))
				{
					// player take damage
					FDamageEvent DamageEvent;
					HitActor->TakeDamage(20.0f, DamageEvent, nullptr, this);

					// fx for impact location
					POnHitTargetBP.Broadcast(ImpactLocation);

					PrimaryActorTick.bCanEverTick = false;
					Destroy();
				}
				else
				{
					if (HitActor == this) return;
					POnHitOtherBP.Broadcast(ImpactLocation);
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
		POnDeadBP.Broadcast(GetActorLocation());
		Destroy();
	}
}

