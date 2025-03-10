// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAMarksmanTrooper.h"
#include "MIRABaseCharacter.h"
#include "MIRACharacterSetting.h"
#include "TrooperAnimInstance.h"
#include "MIRAAIController.h"
#include "MIRAHPBarWidget.h"
#include "MIRAProjectile.h"
#include "MIRAEnemyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMIRAMarksmanTrooper::AMIRAMarksmanTrooper()
{
	// [DONT HAVE TO] Skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MARKSMANTROOPER(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (SK_MARKSMANTROOPER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MARKSMANTROOPER.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MARKSMANTROOPER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooper/MarksmanTrooperAnimBlueprint.MarksmanTrooperAnimBlueprint_C"));
	if (MARKSMANTROOPER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MARKSMANTROOPER_ANIM.Class);
	}

	// Bullet projectile and sfx
	static ConstructorHelpers::FClassFinder<AMIRAProjectile>
		MMTROOPER_PROJECTILE(TEXT("/Game/MIRA/Characters/Blueprints/BP_MMTrooperProjectile.BP_MMTrooperProjectile_C"));
	if (MMTROOPER_PROJECTILE.Succeeded())
	{
		BulletClass = MMTROOPER_PROJECTILE.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SFX_TROOPERFIRE(TEXT("/Game/MIRA/Audio/SQ_SFX_TrooperFire.SQ_SFX_TrooperFire"));
	if (SFX_TROOPERFIRE.Succeeded())
	{
		SFX_TrooperFire = SFX_TROOPERFIRE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		EFX_MUZZLEFIRE(TEXT("/Game/ParagonWraith/FX/Particles/Abilities/Primary/FX/P_Wraith_Primary_MuzzleFlash.P_Wraith_Primary_MuzzleFlash"));
	if (EFX_MUZZLEFIRE.Succeeded())
	{
		EFX_MuzzleFire = EFX_MUZZLEFIRE.Object;
	}
	
	// ai controller
	ConstructorHelpers::FClassFinder<AMIRAAIController> 
		BPCLASS_MMTROOPERAICONTROLLER(TEXT("/Game/MIRA/Characters/AI/BP_MMTrooperAIController.BP_MMTrooperAIController_C"));
	if (BPCLASS_MMTROOPERAICONTROLLER.Succeeded())
	{
		AIControllerClass = BPCLASS_MMTROOPERAICONTROLLER.Class;
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

#pragma region HPBar UI
	// HPBar UI widget
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> 
		UI_HPBAR(TEXT("/Game/MIRA/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UI_HPBAR.Class);
		HPBar->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	HPBar->SetHiddenInGame(true);
#pragma endregion

	// collision setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	//
	IsAttacking = false;

	// enemy index
	EnemyIndex = FMath::RandRange(0, 2);
}

void AMIRAMarksmanTrooper::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MMTrooperAnim = Cast<UTrooperAnimInstance>(GetMesh()->GetAnimInstance());
	MIRACHECK(nullptr != MMTrooperAnim);
	if (!MMTrooperAnim) return;
	MMTrooperAnim->OnMontageEnded.AddDynamic(this, &AMIRAMarksmanTrooper::OnAttackMontageEnded);
}

float AMIRAMarksmanTrooper::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// if dmg valid, play hit anim
	if (FinalDamage > 0.0f)
	{
		if (MMTrooperAnim)	MMTrooperAnim->PlayHitMontage();
	}

	// TO DO: STAT 기반 판별해서
	EnemyStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD)
	{
		OnDead.Broadcast(DamageCauser);
	}
	return FinalDamage;
}

void AMIRAMarksmanTrooper::Attack()
{
	if (!IsAttacking)
	{
		// handling by attack montage in anim instance
		MMTrooperAnim->PlayAttackMontage();
		IsAttacking = true;
	}
	else
	{
		MIRALOG(Warning, TEXT("Already attacking, skipping new attack."));
	}
}

void AMIRAMarksmanTrooper::SetTarget(AMIRAPlayerCharacter* TargetActor)
{
	Target = TargetActor;
}

void AMIRAMarksmanTrooper::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	MIRALOG(Warning, TEXT("[AMIRAMarksmanTrooper::OnAttackMontageEnded]"));
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMIRAMarksmanTrooper::SetCharacterState(ECharacterState NewState)
{
	Super::SetCharacterState(NewState);

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		MIRALOG(Warning, TEXT("LOADING : HPBar->SetHiddenInGame(true)"));
		HPBar->SetHiddenInGame(true);
		break;
	}
	case ECharacterState::READY:
	{
		MIRALOG(Warning, TEXT("READY : HPBar->SetHiddenInGame(false)"));
		HPBar->SetHiddenInGame(false);

		UMIRAHPBarWidget* CharacterWidget = Cast<UMIRAHPBarWidget>(HPBar->GetUserWidgetObject());
		if (nullptr != CharacterWidget)
		{
			CharacterWidget->BindCharacterStat(EnemyStat);
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		if (MMTrooperAnim)	MMTrooperAnim->SetDeadAnim();
		MIRALOG(Warning, TEXT("DEAD : HPBar->SetHiddenInGame(true)"));
		HPBar->SetHiddenInGame(true);
		break;
	}
	default:
		break;
	}
}

void AMIRAMarksmanTrooper::BeginPlay()
{
	Super::BeginPlay();

	// 임시
	//TrooperStat->SetNewStat("MarksmanTrooper");

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->TrooperMeshAssets[FMath::RandRange(0, 2)];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));
}

void AMIRAMarksmanTrooper::FireProjectile()
{
	// bullet
	auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
	// set owner
	Bullet->Owner = this;

	auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (nullptr == TargetPlayer)	return;

	FVector BulletSpawnLocation = GetMesh()->GetSocketLocation(TEXT("Muzzle_01"));
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
	UGameplayStatics::PlaySoundAtLocation(this, SFX_TrooperFire, BulletSpawnLocation);

}
