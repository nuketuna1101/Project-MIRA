// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossIvan.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAAIController.h"
#include "MIRAProjectile.h"
#include "MIRACharacterSetting.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMIRABossIvan::AMIRABossIvan()
{
	// [DONT HAVE TO] Skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_IVAN(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant"));
	if (SK_IVAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_IVAN.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		IVAN_ANIM(TEXT("/Game/MIRA/Characters/Animations/Ivan/ABP_Ivan.ABP_Ivan_C"));
	if (IVAN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(IVAN_ANIM.Class);
	}

	// ai controller
	ConstructorHelpers::FClassFinder<AMIRAAIController>
		BPCLASS_IVANAICONTROLLER(TEXT("/Game/MIRA/Characters/AI/BP_IvanAIController.BP_IvanAIController_C"));
	if (BPCLASS_IVANAICONTROLLER.Succeeded())
	{
		AIControllerClass = BPCLASS_IVANAICONTROLLER.Class;
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// setting bullet
	static ConstructorHelpers::FClassFinder<AMIRAProjectile>
		IVAN_PROJECTILE(TEXT("/Game/MIRA/Characters/Blueprints/BP_IvanHomingProjectile.BP_IvanHomingProjectile_C"));
	if (IVAN_PROJECTILE.Succeeded())
	{
		BulletClass = IVAN_PROJECTILE.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_IVANFIRE(TEXT("/Game/MIRA/Audio/SQ_SFX_IvanFire.SQ_SFX_IvanFire"));
	if (SFX_IVANFIRE.Succeeded())
	{
		SFX_IvanFire = SFX_IVANFIRE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_MUZZLEFIRE(TEXT("/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Primary/FX/P_Revenant_Primary_MuzzleFlash.P_Revenant_Primary_MuzzleFlash"));
	if (EFX_MUZZLEFIRE.Succeeded())
	{
		EFX_MuzzleFire = EFX_MUZZLEFIRE.Object;
	}



	// collision setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
}

void AMIRABossIvan::FireHomings()
{
	// bullet
	auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
	// set owner
	Bullet->Owner = this;

	auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (nullptr == TargetPlayer)	return;

	FVector BulletSpawnLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01_0"));
	FVector TargetLocation = TargetPlayer->GetActorLocation();
	TargetLocation.Z = BulletSpawnLocation.Z;
	FVector BulletDir = (TargetLocation - BulletSpawnLocation).GetSafeNormal();

	Bullet->SetActorLocation(BulletSpawnLocation);

	UProjectileMovementComponent* ProjectileMovement = Bullet->FindComponentByClass<UProjectileMovementComponent>();
	if (ProjectileMovement)
	{
		FVector BulletVel = BulletDir * 800.0f;
		ProjectileMovement->SetVelocityInLocalSpace(BulletVel);
	}

	// FX: EFX and SFX
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_MuzzleFire, BulletSpawnLocation, BulletDir.Rotation());
	UGameplayStatics::PlaySoundAtLocation(this, SFX_IvanFire, BulletSpawnLocation);	
}

void AMIRABossIvan::BeginPlay()
{
	Super::BeginPlay();

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->BossAssets[0];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));

	// set player as Target
	auto TargetPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	Target = Cast<AMIRAPlayerCharacter>(TargetPawn);
}
