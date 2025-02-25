// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABaseCharacter.h"
#include "MIRAAnimInstance.h"
#include "MIRABlade.h"
#include "MIRAPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"
#include "MIRAPlayerController.h"
#include "MIRACharacterSetting.h"

/*
// Sets default values
AMIRABaseCharacter::AMIRABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create defaultsubobject
	CharacterStat = CreateDefaultSubobject<UMIRACharacterStatComponent>(TEXT("CharacterStat"));

	// transform setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// setting for mesh and animations
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_KALLARI(TEXT("/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari"));
	if (SK_KALLARI.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KALLARI.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MIRAPLAYER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAnimBlueprint.MPlayerAnimBlueprint_C"));
	if (MIRAPLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MIRAPLAYER_ANIM.Class);
	}

	// jump velocity
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// setting for collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MIRACharacter"));

	// bool varaibles
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
}

void AMIRABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// initializing for anim montage
	MIRAAnim = Cast<UMIRAAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MIRAAnim) return;

	// binding logics to anim notify
	MIRAAnim->OnSaveAttackCheck.AddLambda([this]() -> void {
		SaveAttackCombo();
		});
	MIRAAnim->OnResetComboCheck.AddLambda([this]() -> void {
		ResetAttackCombo();
		});
	MIRAAnim->OnAttackHitCheck.AddUObject(this, &AMIRACharacter::AttackCheck);
}

void AMIRABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}
*/