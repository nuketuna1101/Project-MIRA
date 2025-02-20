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

	// setting for collision channel
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MIRACharacter"));

	// bool varaible for move
	bCannotMove = false;
	// bool varaible for dodge
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

	// character stat
	CharacterStat->SetNewLevel(1);

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
	// implementation of aiming by lerp
	if (bIsAiming)
	{
		float CurrentFOV = Camera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 75.0f, DeltaTime, 3.0f);
		Camera->SetFieldOfView(NewFOV);
	}
	else
	{
		float CurrentFOV = Camera->FieldOfView;
		float NewFOV = FMath::FInterpTo(CurrentFOV, 90.0f, DeltaTime, 3.0f);
		Camera->SetFieldOfView(NewFOV);
	}

	//
	if (bIsDodging)
	{

	}
	else
	{

	}


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
			bIsDashing = false; // 충돌 시 대쉬 종료
		}

		SetActorLocation(TargetLocation);
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
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AMIRACharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Execute"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Execute);

	// bindings for axis mapping
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMIRACharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMIRACharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMIRACharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMIRACharacter::LookUp);
}

void AMIRACharacter::OnAssetLoadCompleted()
{
	//AssetStreamingHandle->ReleaseHandle();
	//TSoftObjectPtr<USkeletalMesh> LoadAssetPath(CharacterAssetToLoad);

	//GetMesh()->SetSkeletalMesh(LoadAssetPath.Get());
	//SetCharacterState(ECharacterState::READY);
}

float AMIRACharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//MIRALOG(Warning, TEXT("Actor: %s / damage: %f"), *GetName(), FinalDamage);

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
	GetCharacterMovement()->AddImpulse(FVector(0.0f, 0.0f, 1.0f) * 20.0f, true);
	GetCharacterMovement()->AddImpulse(NormalDodgeDirection * 400.0f, true);
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
	//MIRALOG(Warning, TEXT("[SaveAttackCombo] called / bSaveAttack : %s"), bSaveAttack ? TEXT("true") : TEXT("false"));
	if (bSaveAttack)
	{
		bSaveAttack = false;
		PerformAttackCombo();
	}
}

void AMIRACharacter::ResetAttackCombo()
{
	bCannotMove = false;
	//MIRALOG(Warning, TEXT("[ResetAttackCombo] called"));
	CurrentComboCount = 0;
	bIsAttacking = false;
	bSaveAttack = false;
}

void AMIRACharacter::AttackCheck()
{
	auto OverlappedEnemies = RightBlade->GetOverlappedEnemies();
	auto OverlappedEnemiesLeft = LeftBlade->GetOverlappedEnemies();
	for (auto Enemy : OverlappedEnemiesLeft)
	{
		OverlappedEnemies.AddUnique(Enemy);
	}

	for (AMIRAEnemyBaseCharacter* Enemy : OverlappedEnemies)
	{
		if (Enemy && Enemy != GetOwner())
		{
			// take damage
			MIRALOG(Warning, TEXT("[AttackCheck]"));

			FDamageEvent DamageEvent;
			Enemy->TakeDamage(20.0f, DamageEvent, nullptr, this);

			// get expected impact point 
			FVector EnemyLocation = Enemy->GetActorLocation();
			OnHitBP.Broadcast(EnemyLocation);
		}
	}
	//
	OverlappedEnemies.Empty();
	RightBlade->FreeAllOverlappedEnemies();
	LeftBlade->FreeAllOverlappedEnemies();
}

AMIRABlade* AMIRACharacter::GetBladeRight()
{
	return RightBlade;
}

AMIRABlade* AMIRACharacter::GetBladeLeft()
{
	return LeftBlade;
}

void AMIRACharacter::Attack()
{
	//MIRALOG(Warning, TEXT("[Attack] called / bIsAttacking : %s"), bIsAttacking ? TEXT("true") : TEXT("false"));
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
	// ranged attack logic
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

void AMIRACharacter::StartDash()
{
	if (bCannotMove || bIsDashing) return;
	
	bIsDashing = true;

	DashDirection = 
		FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X) * GetInputAxisValue("UpDown") +
		FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y) * GetInputAxisValue("LeftRight");
	if (DashDirection.IsNearlyZero())
	{
		bIsDashing = false;
		return;
	}

	DashDirection.Normalize();
	CurrentDashDistance = 0.0f;

	OnStartDashBP.Broadcast(GetActorLocation());
}
