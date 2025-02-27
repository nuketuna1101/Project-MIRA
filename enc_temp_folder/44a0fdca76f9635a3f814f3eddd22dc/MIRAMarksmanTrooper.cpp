// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAMarksmanTrooper.h"
#include "MIRABaseCharacter.h"
#include "MIRACharacterSetting.h"
#include "TrooperAnimInstance.h"
#include "TrooperAIController.h"
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

	// temp bullet
	static ConstructorHelpers::FObjectFinder<UBlueprint>
		blueprint_finder(TEXT("Blueprint'/Game/MIRA/Characters/Blueprints/BP_Tempbullet.BP_Tempbullet'"));
		//blueprint_finder(TEXT("Blueprint'/Game/MIRA/Characters/Blueprints/BP_TrooperBullet.BP_TrooperBullet'"));
	BulletClass = (UClass*)blueprint_finder.Object->GeneratedClass;
	/////
	if (BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletClass: %s"), *BulletClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BulletClass is null!"));
	}
	/////
	// ai controller
	AIControllerClass = ATrooperAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// collision setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	//
	IsAttacking = false;
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
	CharacterStat->SetDamage(FinalDamage);
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
		//MIRAAnim->PlayAttackMontage();
		IsAttacking = true;

		// bullet
		auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
		auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (nullptr == TargetPlayer)	return;

		FVector TrooperLocation = GetActorLocation();
		TrooperLocation.Z += 50.0f;
		FVector TargetLocation = TargetPlayer->GetActorLocation();
		TargetLocation.Z = TrooperLocation.Z;
		FVector BulletDir = (TargetLocation - TrooperLocation).GetSafeNormal();


		Bullet->SetActorLocation(TrooperLocation);

		UProjectileMovementComponent* ProjectileMovement = Bullet->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			FVector BulletVel = BulletDir * 800.0f;
			ProjectileMovement->SetVelocityInLocalSpace(BulletVel);
		}
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
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

void AMIRAMarksmanTrooper::BeginPlay()
{
	Super::BeginPlay();

	// 임시
	//TrooperStat->SetNewStat("MarksmanTrooper");

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	//CharacterAssetToLoad = DefaultSetting->TrooperAssets[0];
	CharacterAssetToLoad = DefaultSetting->TrooperAssets[FMath::RandRange(0, 2)];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));
}