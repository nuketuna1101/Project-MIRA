// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossIvan.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAAIController.h"
#include "MIRAProjectile.h"
#include "MIRACharacterSetting.h"
#include "IvanAnimInstance.h"
#include "BoomGround.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnrealMathUtility.h"

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

#pragma region BP assets
	static ConstructorHelpers::FClassFinder<AMIRAProjectile>
		IVAN_PROJECTILE(TEXT("/Game/MIRA/Characters/Blueprints/BP_IvanHomingProjectile.BP_IvanHomingProjectile_C"));
	if (IVAN_PROJECTILE.Succeeded())
	{
		BulletClass = IVAN_PROJECTILE.Class;
	}
	static ConstructorHelpers::FClassFinder<ABoomGround>
		IVAN_BOOMGROUND(TEXT("/Game/MIRA/Characters/Blueprints/BP_BoomGround.BP_BoomGround_C"));
	if (IVAN_BOOMGROUND.Succeeded())
	{
		BGClass = IVAN_BOOMGROUND.Class;
	}
#pragma endregion

#pragma region EFX assets
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_MUZZLEFIRE(TEXT("/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Primary/FX/P_Revenant_Primary_MuzzleFlash.P_Revenant_Primary_MuzzleFlash"));
	if (EFX_MUZZLEFIRE.Succeeded())
	{
		EFX_MuzzleFire = EFX_MUZZLEFIRE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_THROWTRAIL(TEXT("/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Wolf/Poison/P_ChargeTrail_Poison.P_ChargeTrail_Poison"));
	if (EFX_THROWTRAIL.Succeeded())
	{
		EFX_ThrowTrail = EFX_THROWTRAIL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_CASTING(TEXT("/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Mark/FX/P_Revenant_Mark_Targeting.P_Revenant_Mark_Targeting"));
	if (EFX_CASTING.Succeeded())
	{
		EFX_Casting = EFX_CASTING.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_DEATHSTARE(TEXT("/Game/ParagonRevenant/FX/Particles/Revenant/Abilities/Ultimate/FX/P_Revenant_Ultimate_Sky_Portal.P_Revenant_Ultimate_Sky_Portal"));
	if (EFX_DEATHSTARE.Succeeded())
	{
		EFX_DeathStare = EFX_DEATHSTARE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_VANISH(TEXT("/Game/ParagonRevenant/FX/Particles/Revenant/P_Revenant_Recall_Finish.P_Revenant_Recall_Finish"));
	if (EFX_VANISH.Succeeded())
	{
		EFX_Vanish = EFX_VANISH.Object;
	}
#pragma endregion

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_IVANFIRE(TEXT("/Game/MIRA/Audio/SQ_SFX_IvanFire.SQ_SFX_IvanFire"));
	if (SFX_IVANFIRE.Succeeded())
	{
		SFX_IvanFire = SFX_IVANFIRE.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_THROW(TEXT("/Game/MIRA/Audio/SQ_SFX_Throw.SQ_SFX_Throw"));
	if (SFX_THROW.Succeeded())
	{
		SFX_Throw = SFX_THROW.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_CASTING(TEXT("/Game/MIRA/Audio/SQ_SFX_Casting.SQ_SFX_Casting"));
	if (SFX_CASTING.Succeeded())
	{
		SFX_Casting = SFX_CASTING.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_DEATHSTARE(TEXT("/Game/MIRA/Audio/SQ_SFX_DeathStare.SQ_SFX_DeathStare"));
	if (SFX_DEATHSTARE.Succeeded())
	{
		SFX_DeathStare = SFX_DEATHSTARE.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_STARTVANISH(TEXT("/Game/MIRA/Audio/SQ_SFX_StartVanish.SQ_SFX_StartVanish"));
	if (SFX_STARTVANISH.Succeeded())
	{
		SFX_StartVanish = SFX_STARTVANISH.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_ENDVANISH(TEXT("/Game/MIRA/Audio/SQ_SFX_EndVanish.SQ_SFX_EndVanish"));
	if (SFX_ENDVANISH.Succeeded())
	{
		SFX_EndVanish = SFX_ENDVANISH.Object;
	}


	// collision setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
}

void AMIRABossIvan::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	IvanAnim = Cast<UIvanAnimInstance>(GetMesh()->GetAnimInstance());
	MIRACHECK(nullptr != IvanAnim);
	if (!IvanAnim) return;
}

void AMIRABossIvan::FireHomings()
{
	if (!Target) return;

	// bullet
	auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
	Bullet->Owner = this;

	FVector BulletSpawnLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_02"));
	FVector TargetLocation = Target->GetActorLocation();
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

void AMIRABossIvan::ThrowBoomGrounds()
{
	// FX: EFX and SFX
	FVector ThrowTrailLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_ThrowTrail, ThrowTrailLocation, FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, SFX_Throw, ThrowTrailLocation);
}

void AMIRABossIvan::LaunchBoomGrounds()
{
	if (Target)
	{
		float BombRadius = 200.0f;
		FVector PlayerLocation = Target->GetActorLocation();
		// perform line trace to get GROUNDED location
		FVector TraceStart = PlayerLocation + FVector(0.0f, 0.0f, 1000.0f);
		FVector TraceEnd = PlayerLocation - FVector(0.0f, 0.0f, 1000.0f);
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Target);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
		{
			FVector SpawnLocation = HitResult.ImpactPoint;
			ABoomGround* Bomb = GetWorld()->SpawnActor<ABoomGround>(BGClass, SpawnLocation, FRotator::ZeroRotator);
		}
		else
		{
			MIRALOG(Error, TEXT("Failed to line trace grounded"));
		}
	}

}

void AMIRABossIvan::CastDeathStare()
{
	// FX: EFX and SFX
	FVector CastingLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_MuzzleFire, CastingLocation, FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, SFX_Casting, CastingLocation);
}

void AMIRABossIvan::DeathStare()
{
	if (!Target) return;

	// FX: EFX and SFX
	FVector DeathStareLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_04"));

	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = DeathStareLocation.Z;

	FRotator DeathStareRotation = (TargetLocation - DeathStareLocation).GetSafeNormal().Rotation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_MuzzleFire, DeathStareLocation, DeathStareRotation);
	UGameplayStatics::PlaySoundAtLocation(this, SFX_DeathStare, DeathStareLocation);
}

void AMIRABossIvan::StartVanish()
{
	// FX: EFX and SFX
	FVector StartVanishLocation = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_MuzzleFire, GetActorLocation(), FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, SFX_StartVanish, StartVanishLocation);

	SetActorHiddenInGame(true);
}

void AMIRABossIvan::EndVanish()
{
	if (!Target) return;

	// FX: EFX and SFX
	FVector TargetLocation = Target->GetActorLocation();
	FVector RandomOffset = FVector(FMath::RandRange(-200.0f, 200.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f);
	FVector FinalLocation = TargetLocation + RandomOffset;
	SetActorLocation(FinalLocation);
	FVector EndVanishLocation = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EFX_MuzzleFire, EndVanishLocation, FRotator::ZeroRotator);
	UGameplayStatics::PlaySoundAtLocation(this, SFX_EndVanish, EndVanishLocation);

	SetActorHiddenInGame(false);
}

void AMIRABossIvan::BeginPlay()
{
	Super::BeginPlay();

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->BossMeshAssets[0];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::OnAssetLoadCompleted));

	// set player as Target
	auto TargetPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	Target = Cast<AMIRAPlayerCharacter>(TargetPawn);
}
