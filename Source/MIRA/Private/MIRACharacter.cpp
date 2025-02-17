// Fill out your copyright notice in the Description page of Project Settings.

#include "MIRACharacter.h"
#include "MIRAAnimInstance.h"
#include "MIRABlade.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMIRACharacter::AMIRACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// create defaultsubobject
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CharacterStat = CreateDefaultSubobject<UMIRACharacterStatComponent>(TEXT("CharacterStat"));

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

	// variable for aim
	bIsAiming = false;

	bIsWalking = false;

	bIsBlocking = false;
}

// Called when the game starts or when spawned
void AMIRACharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// weapon attach
	FName WeaponRightSocket(TEXT("blade_right_socket"));
	FName WeaponLeftSocket(TEXT("blade_left_socket"));

	RightBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	//auto RightBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != RightBlade)
	{
		RightBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		RightBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponRightSocket);
	}
	LeftBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	//auto LeftBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != LeftBlade)
	{
		LeftBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		LeftBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponLeftSocket);
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

	//
		// 줌 기능 구현
	if (bIsAiming)
	{
		// 현재 FOV 값을 목표 FOV 값으로 부드럽게 보간
		float CurrentFOV = Camera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 75.0f, DeltaTime, 3.0f);
		Camera->SetFieldOfView(NewFOV);
	}
	else
	{
		// 현재 FOV 값을 초기 FOV 값으로 부드럽게 보간
		float CurrentFOV = Camera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 90.0f, DeltaTime, 3.0f);
		Camera->SetFieldOfView(NewFOV);
	}
}

void AMIRACharacter::PostInitializeComponents()
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

// Called to bind functionality to input
void AMIRACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings for action mapping
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AMIRACharacter::StartAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &AMIRACharacter::StopAim);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Block);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Dodge);
	PlayerInputComponent->BindAction(TEXT("Execute"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Execute);



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
		if (MIRAAnim)	MIRAAnim->PlayHitMontage();
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
	if (NewAxisValue == 0)
	{
		bIsWalking = false;
		return;
	}
	bIsWalking = true;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AMIRACharacter::LeftRight(float NewAxisValue)
{
	if (bCannotMove) return;
	if (NewAxisValue == 0)
	{
		bIsWalking = false;
		return;
	}
	bIsWalking = true;
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

void AMIRACharacter::Block()
{
	bIsBlocking = true;
}

void AMIRACharacter::Dodge()
{
	// TO DO: bCannotMove 설정해줘야 함

	// play dodge anim
	MIRAAnim->PlayDodgeMontage();

	///////////
	// 이동 방향 벡터 계산
	FVector DodgeDirection = GetCharacterMovement()->GetLastInputVector();
	if (DodgeDirection.IsZero())
	{
		DodgeDirection = GetActorForwardVector();
	}
	FVector NormalDodgeDirection = DodgeDirection.GetSafeNormal();

	// 회피/구르기 애니메이션 재생 (애니메이션 블루프린트에서 처리)

	// 회피/구르기 이동
	GetCharacterMovement()->AddImpulse(NormalDodgeDirection * 200.0f, true);
	//LaunchCharacter(-GetActorForwardVector() * 300.0f + FVector(0.0f, 0.0f, 100.0f), false, false);
}

void AMIRACharacter::Execute()
{
	MIRAAnim->PlayExecuteMontage();
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

	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + 50.0f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), 
		Center, 
		HalfHeight, 
		50.0f, 
		CapsuleRot, 
		DrawColor, 
		false, 
		DebugLifeTime);


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

AMIRABlade* AMIRACharacter::GetBladeRight()
{
	return RightBlade;
}

AMIRABlade* AMIRACharacter::GetBladeLeft()
{
	return LeftBlade;
}

bool AMIRACharacter::IsWalking()
{
	return bIsWalking;
}

void AMIRACharacter::Attack()
{
	MIRALOG(Warning, TEXT("[Attack] called / bIsAttacking : %s"), bIsAttacking? TEXT("true") : TEXT("false"));
	if (bIsAiming)
	{
		AttackRange();
	}
	else
	{
		AttackMelee();
	}
}

void AMIRACharacter::StartBlock()
{
	bIsBlocking = true;
}

void AMIRACharacter::StopBlock()
{
	bIsBlocking = false;
}

void AMIRACharacter::AttackMelee()
{
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

void AMIRACharacter::AttackRange()
{
}

void AMIRACharacter::StartAim()
{
	bIsAiming = true;

	SpringArmLength = 400.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	SpringArm->SocketOffset = FVector(100.0f, 60.0f, 0.0f);
}

void AMIRACharacter::StopAim()
{
	bIsAiming = false;

	SpringArmLength = 450.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	SpringArm->SocketOffset = FVector::ZeroVector;
}