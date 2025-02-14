// Fill out your copyright notice in the Description page of Project Settings.

#include "MIRACharacter.h"
#include "MIRAAnimInstance.h"
#include "MIRABlade.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMIRACharacter::AMIRACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// create defaultsubobject
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// hierarchy
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// transform setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// mesh
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

	// camera mode
	SetCameraMode(ECameraMode::FreeTPS);
	SpringArmRotationSpeed = 10.0f;
	SpringArmLengthSpeed = 3.0f;

	// jump velocity
	GetCharacterMovement()->JumpZVelocity = 500.0f;

	// bool variable for attack
	//IsAttacking = false;

	//// attack combo
	//MaxCombo = 2;
	//AttackEndComboState();

	// setting for collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MIRACharacter"));

	// bool varaible for move
	bCannotMove = false;

	// bool varaible for dodge
	bIsDodgeMode = false;

	// varaibles for attack combo
	MaxCombo = 5;
	bIsAttacking = false;
	bSaveAttack = false;
}

// Called when the game starts or when spawned
void AMIRACharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// weapon attach
	FName WeaponRightSocket(TEXT("blade_right_socket"));
	FName WeaponLeftSocket(TEXT("blade_left_socket"));

	auto CurRightBlade = GetWorld()->SpawnActor<AMIRABlade>
				(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurRightBlade)
	{
		CurRightBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		CurRightBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponRightSocket);
	}
	auto CurLeftBlade = GetWorld()->SpawnActor<AMIRABlade>
		(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurLeftBlade)
	{
		CurLeftBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		CurLeftBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponLeftSocket);
	}
}

void AMIRACharacter::SetCameraMode(ECameraMode CameraMode)
{
	CurrentControlMode = CameraMode;

	switch (CurrentControlMode)
	{
	case ECameraMode::FreeTPS:
		SpringArmLength = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true; 
		SpringArm->bInheritRoll = true; 
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true; 
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}
}

// Called every frame
void AMIRACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// spring arm interpolation
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength,
		SpringArmLength, DeltaTime, SpringArmLengthSpeed);
}

void AMIRACharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// initializing for anim montage
	MIRAAnim = Cast<UMIRAAnimInstance>(GetMesh()->GetAnimInstance());
	if (!MIRAAnim) return;

	MIRAAnim->OnSaveAttackCheck.AddLambda([this]() -> void {
		SaveAttackCombo();
	});
	MIRAAnim->OnResetComboCheck.AddLambda([this]() -> void {
		ResetAttackCombo();
	});
}

// Called to bind functionality to input
void AMIRACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings for action mapping
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Dodge);


	// bindings for axis mapping
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMIRACharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMIRACharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMIRACharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMIRACharacter::LookUp);
}

float AMIRACharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	MIRALOG(Warning, TEXT("Actor: %s / damage: %f"), *GetName(), FinalDamage);

	// TMP: 일단은 조금이라도 피격 당하면 사망으로
	if (FinalDamage > 0.0f)
	{
		// TO DO: 피격 시 피격 애니메이션 재생
		//if (MIRAAnim)			MIRAAnim->PlayHitMontage();
	}

	// TO DO: STAT 기반 판별해서
	/*
	CharacterStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD)
	{
		OnDead.Broadcast(DamageCauser);
		if (EventInstigator->IsPlayerController())
		{
			ABPlayerController = Cast<AABPlayerController>(EventInstigator);
			ABPlayerController->NPCKill(this);
		}
	}
	*/

	return FinalDamage;
}

void AMIRACharacter::UpDown(float NewAxisValue)
{
	if (bCannotMove) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AMIRACharacter::LeftRight(float NewAxisValue)
{
	if (bCannotMove) return;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AMIRACharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AMIRACharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMIRACharacter::Dodge()
{
	// TO DO: bCannotMove 설정해줘야 함

	// play dodge anim
	MIRAAnim->PlayDodgeMontage();

	UCharacterMovementComponent* MIRACharMovement = GetCharacterMovement();
	LaunchCharacter(-GetActorForwardVector() * 300.0f + FVector(0.0f, 0.0f, 100.0f), false, false);
}

void AMIRACharacter::PerformAttackCombo()
{
	OnAttackEndBP.Broadcast();
	bCannotMove = true;
	CurrentComboCount = (CurrentComboCount + 1) % 5;
	MIRAAnim->PlayAttackComboMontage(CurrentComboCount);
}

void AMIRACharacter::SaveAttackCombo()
{
	MIRALOG(Warning, TEXT("[SaveAttackCombo] called / bSaveAttack : %s"), bSaveAttack ? TEXT("true") : TEXT("false"));
	if (bSaveAttack)
	{
		bSaveAttack = false;
		PerformAttackCombo();
	}
}

void AMIRACharacter::ResetAttackCombo()
{
	bCannotMove = false;
	MIRALOG(Warning, TEXT("[ResetAttackCombo] called"));
	CurrentComboCount = 0;
	bIsAttacking = false;
	bSaveAttack = false;
}

void AMIRACharacter::AttackCheck()
{
	float FinalAttackRange = 120.0f; //GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),		// tmp radius
		Params);

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			OnHitBP.Broadcast(HitResult.ImpactPoint);
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(200.0f, DamageEvent, GetController(), this);
		}
	}
}

void AMIRACharacter::Attack()
{
	MIRALOG(Warning, TEXT("[Attack] called / bIsAttacking : %s"), bIsAttacking? TEXT("true") : TEXT("false"));
	if (bIsAttacking)
	{
		bSaveAttack = true;
		return;
	}
	else
	{
		bIsAttacking = true;
		PerformAttackCombo();
	}
}