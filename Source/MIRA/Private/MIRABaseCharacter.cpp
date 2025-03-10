// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABaseCharacter.h"
#include "MIRAAnimInstance.h"
#include "MIRAPlayerState.h"
#include "MIRAEnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "MIRAPlayerController.h"
#include "MIRAHUDWidget.h"
#include "MIRACharacterSetting.h"
#include "MIRAGameMode.h"
#include "MIRAAIController.h"

AMIRABaseCharacter::AMIRABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create defaultsubobject
	CharacterStat = CreateDefaultSubobject<UMIRACharacterStatComponent>(TEXT("CharacterStat"));
	EnemyStat = CreateDefaultSubobject<UMIRAEnemyStatComponent>(TEXT("EnemyStat"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// [TO DO] springarm and camera

	// [TO DO] SetSkeletalMesh

	// [TO DO] setting for animations

	// jump velocity
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// setting for collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MIRACharacter"));

	// bool varaible for actions
	bCannotMove = false;
	bIsDodging = false;

	// varaibles for attack combo
	MaxCombo = 5;
	bIsAttacking = false;
	bSaveAttack = false;

	// variable for aim
	bIsAiming = false;
	bIsWalking = false;
	bIsBlocking = false;
	bIsDashing = false;

	// dash variable
	MaxDashDistance = 400.0f;
	DashSpeed = 800.0f;
	CurrentDashDistance = 0.0f;

	// character state control
	SetActorHiddenInGame(true);
	SetCanBeDamaged(false);

	// dead timer
	DeadTimer = 5.0f;
}

void AMIRABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// [TO DO] spring arm interpolation
	// [TO DO] implementation of aiming by lerp
	
	// implementation of dash
	if (bIsDashing)
	{
		float DashDelta = DashSpeed * DeltaTime;
		CurrentDashDistance += DashDelta;

		if (CurrentDashDistance >= MaxDashDistance)
		{
			CurrentDashDistance = MaxDashDistance;
			bIsDashing = false;
		}

		FVector TargetLocation = GetActorLocation() + DashDirection * DashDelta;

		// Sweep 테스트를 통해 충돌 여부 확인
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 자기 자신은 무시

		bool bHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			GetActorLocation(),
			TargetLocation,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(30.0f),
			QueryParams); // 충돌체 크기 조절

		if (bHit)
		{
			// 충돌 발생 시, 충돌 지점까지 이동
			TargetLocation = HitResult.ImpactPoint;
			bIsDashing = false;
		}

		SetActorLocation(TargetLocation);
	}
}

void AMIRABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// [TO DO] initializing for anim montage

	// [TO DO] binding logics to anim notify
}

void AMIRABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// [TO DO] bindings for action mapping
	// [TO DO] bindings for axis mapping
}

float AMIRABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// if dmg valid, play hit anim
	if (FinalDamage > 0.0f)
	{
		if (MIRAAnim)
		{
			MIRALOG(Warning, TEXT("[AMIRABaseCharacter::TakeDamage] PlayHitMontage"));
			MIRAAnim->PlayHitMontage();
		}
		else
		{
			MIRALOG(Warning, TEXT("[AMIRABaseCharacter::TakeDamage] no MIRAAnim"));
		}
	}

	// TO DO: STAT 기반 판별해서
	if (bIsPlayer)
		CharacterStat->SetDamage(FinalDamage);
	else
		EnemyStat->SetDamage(FinalDamage);


	if (CurrentState == ECharacterState::DEAD)
	{
		OnDead.Broadcast(DamageCauser);
		if (nullptr != EventInstigator) 
		{
			if (EventInstigator->IsPlayerController())
			{
				MIRAPlayerController = Cast<AMIRAPlayerController>(EventInstigator);
				MIRACHECK(nullptr != MIRAPlayerController, 0.0f);
				//MIRAPlayerController->NPCKill(this);
			}
		}
	}
	return FinalDamage;
}

void AMIRABaseCharacter::SetCharacterState(ECharacterState NewState)
{
	MIRACHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::PREINIT:
		break;
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(MIRAPlayerController);
			MIRAPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);
			auto MIRAPlayerState = Cast<AMIRAPlayerState>(GetPlayerState());
			MIRACHECK(nullptr != MIRAPlayerState);
			CharacterStat->SetNewLevel(MIRAPlayerState->GetCharacterLevel());
		}
		else
		{
			EnemyStat->SetNewStat(EnemyIndex);
			auto MIRAGameMode = Cast<AMIRAGameMode>(GetWorld()->GetAuthGameMode());
			MIRACHECK(nullptr != MIRAGameMode);
		}

		SetActorHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnZeroHP.AddLambda([this]()->void
			{
				SetCharacterState(ECharacterState::DEAD);
			});
		EnemyStat->OnEnemyHPZero.AddLambda([this]()->void
			{
				SetCharacterState(ECharacterState::DEAD);
			});

		if (bIsPlayer)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(MIRAPlayerController);
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			if (MIRAAIController)
				MIRAAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		if (MIRAAnim)	MIRAAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(MIRAPlayerController);
		}
		else
		{
			if (MIRAAIController) 
			{
				MIRAAIController->StopAI();
			}
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
			FTimerDelegate::CreateLambda([this]()->void {
				if (bIsPlayer)
				{
					//ABPlayerController->ShowResultUI();
					//ABPlayerController->RestartLevel();
				}
				else
				{
					Destroy();
				}
				}), DeadTimer, false);

		break;
	}
	default:
		break;
	}
}

void AMIRABaseCharacter::Attack()
{
}

void AMIRABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// [TO DO] weapon attach

	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		MIRAPlayerController = Cast<AMIRAPlayerController>(GetController());
		MIRACHECK(nullptr != MIRAPlayerController);
	}
	else
	{
		MIRAAIController = Cast<AMIRAAIController>(GetController());
		MIRACHECK(nullptr != MIRAAIController);
	}
	SetCharacterState(ECharacterState::LOADING);

	// [TO DO] Asset Loading
}

void AMIRABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetCharacterMovement()->MaxWalkSpeed
		= (IsPlayerControlled() ? 600.0f : 400.0f);
}

void AMIRABaseCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadAssetPath(CharacterAssetToLoad);
	MIRACHECK(LoadAssetPath.IsValid());
	GetMesh()->SetSkeletalMesh(LoadAssetPath.Get());
	SetCharacterState(ECharacterState::READY);
}