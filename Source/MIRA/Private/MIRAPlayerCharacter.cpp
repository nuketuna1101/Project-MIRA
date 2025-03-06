// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPlayerCharacter.h"
#include "MIRABlade.h"
#include "MIRAGameMode.h"
#include "MIRAProjectile.h"
#include "MIRAAnimInstance.h"
#include "MIRACharacterSetting.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMIRAPlayerCharacter::AMIRAPlayerCharacter()
{
	// [TO DO] springarm and camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SetCameraMode(ETempCameraMode::FreeTPS);
	SpringArmRotationSpeed = 10.0f;
	SpringArmLengthSpeed = 3.0f;

	// [TO DO] [DONT HAVE TO] SetSkeletalMesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_KALLARI(TEXT("/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari"));
	if (SK_KALLARI.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_KALLARI.Object);
	}

	// [TO DO] setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MIRAPLAYER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAnimBlueprint.MPlayerAnimBlueprint_C"));
	if (MIRAPLAYER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MIRAPLAYER_ANIM.Class);
	}

	// bulletclass setting
	static ConstructorHelpers::FObjectFinder<UBlueprint>
		blueprint_finder(TEXT("Blueprint'/Game/MIRA/Characters/Blueprints/BP_MIRAPlayerProjectile.BP_MIRAPlayerProjectile'"));
	BulletClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

void AMIRAPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// [TO DO] spring arm interpolation
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength,
		SpringArmLength, DeltaTime, SpringArmLengthSpeed);
	// [TO DO] implementation of aiming by lerp
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
}

void AMIRAPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// test
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		// 애니메이션 인스턴스의 클래스 이름 출력
		MIRALOG(Warning, TEXT("Animation Instance Class: %s"), *AnimInstance->GetClass()->GetName());
		// 애니메이션 인스턴스가 UMIRAAnimInstance 타입인지 확인
		if (AnimInstance->IsA<UMIRAAnimInstance>())
		{
			MIRALOG(Warning, TEXT("Animation Instance is UMIRAAnimInstance"));
		}
		else
		{
			MIRALOG(Warning, TEXT("Animation Instance is NOT UMIRAAnimInstance"));
		}
		// 캐스팅 시도 및 결과 확인
		MIRAAnim = Cast<UMIRAAnimInstance>(AnimInstance);
		if (MIRAAnim)
		{
			MIRALOG(Warning, TEXT("Casting to UMIRAAnimInstance successful"));
		}
		else
		{
			MIRALOG(Warning, TEXT("Casting to UMIRAAnimInstance failed"));
		}
	}
	else
	{
		MIRALOG(Warning, TEXT("Animation Instance is NOT UMIRAAnimInstance"));
	}
	// ===


	// initializing for anim montage
	MIRAAnim = Cast<UMIRAAnimInstance>(GetMesh()->GetAnimInstance());
	MIRACHECK(nullptr != MIRAAnim);
	if (!MIRAAnim) return;

	// binding logics to anim notify
	MIRAAnim->OnSaveAttackCheck.AddLambda([this]() -> void {
		SaveAttackCombo();
		});
	MIRAAnim->OnResetComboCheck.AddLambda([this]() -> void {
		ResetAttackCombo();
		});
	MIRAAnim->OnAttackHitCheck.AddUObject(this, &AMIRAPlayerCharacter::AttackCheck);
	MIRAAnim->OnThrowRangedCheck.AddLambda([this]() -> void {
		ThrowRanged();
		});
}

void AMIRAPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings for action mapping
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::StartAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &AMIRAPlayerCharacter::StopAim);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Block"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::Block);
	PlayerInputComponent->BindAction(TEXT("Dodge"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::Dodge);
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::StartDash);
	PlayerInputComponent->BindAction(TEXT("Execute"), EInputEvent::IE_Pressed, this, &AMIRAPlayerCharacter::Execute);

	// bindings for axis mapping
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMIRAPlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMIRAPlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMIRAPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMIRAPlayerCharacter::LookUp);
}

void AMIRAPlayerCharacter::Attack()
{
	if (bIsAiming)
	{
		AttackRange();
	}
	else
	{
		AttackMelee();
	}
}

void AMIRAPlayerCharacter::StartBlock()
{
	bIsBlocking = true;
}

void AMIRAPlayerCharacter::StopBlock()
{
	bIsBlocking = false;
}

void AMIRAPlayerCharacter::StartAim()
{
	bIsAiming = true;

	SpringArmLength = 400.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	SpringArm->SocketOffset = FVector(100.0f, 60.0f, 0.0f);
}

void AMIRAPlayerCharacter::StopAim()
{
	bIsAiming = false;

	SpringArmLength = 450.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	SpringArm->SocketOffset = FVector::ZeroVector;
}

void AMIRAPlayerCharacter::StartDash()
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

void AMIRAPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// [TO DO] weapon attach
	FName WeaponRightSocket(TEXT("blade_right_socket"));
	FName WeaponLeftSocket(TEXT("blade_left_socket"));

	RightBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != RightBlade)
	{
		RightBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		RightBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponRightSocket);
	}
	LeftBlade = GetWorld()->SpawnActor<AMIRABlade>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != LeftBlade)
	{
		LeftBlade->SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
		LeftBlade->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponLeftSocket);
	}

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->CharacterMeshAssets[0];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));
}

void AMIRAPlayerCharacter::SetCameraMode(ETempCameraMode CameraMode)
{
	CurrentControlMode = CameraMode;

	switch (CurrentControlMode)
	{
	case ETempCameraMode::FreeTPS:
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

void AMIRAPlayerCharacter::AttackMelee()
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

void AMIRAPlayerCharacter::AttackRange()
{
	// ranged attack logic
	/* play attack range anim */
	/* reduce projectile count */
	if (MIRAAnim) MIRAAnim->PlayRangedAttackMontage();
	bCannotMove = true;
}

void AMIRAPlayerCharacter::PerformAttackCombo()
{
	OnAttackEndBP.Broadcast();
	bCannotMove = true;
	CurrentComboCount = (CurrentComboCount + 1) % 5;
	MIRACHECK(nullptr != MIRAAnim);
	if(MIRAAnim) MIRAAnim->PlayAttackComboMontage(CurrentComboCount);
}

void AMIRAPlayerCharacter::SaveAttackCombo()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		PerformAttackCombo();
	}
}

void AMIRAPlayerCharacter::ResetAttackCombo()
{
	bCannotMove = false;
	CurrentComboCount = 0;
	bIsAttacking = false;
	bSaveAttack = false;
}

void AMIRAPlayerCharacter::AttackCheck()
{
	auto OverlappedEnemies = RightBlade->GetOverlappedEnemies();
	auto OverlappedEnemiesLeft = LeftBlade->GetOverlappedEnemies();
	for (auto Enemy : OverlappedEnemiesLeft)
	{
		OverlappedEnemies.AddUnique(Enemy);
	}

	for (AMIRABaseCharacter* Enemy : OverlappedEnemies)
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

void AMIRAPlayerCharacter::ThrowRanged()
{
	MIRALOG(Warning, TEXT("ThrowRanged"));
	bCannotMove = false;

	// bullet
	//auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));

	AMIRAGameMode* GameMode = Cast<AMIRAGameMode>(UGameplayStatics::GetGameMode(GetWorld()));


	if (nullptr == GameMode)
	{
		MIRALOG(Warning, TEXT("11"));

	}
	else
	{
		MIRALOG(Warning, TEXT("22"));

	}

	if (nullptr == GameMode->ProjectilePool.Get())
	{
		MIRALOG(Warning, TEXT("33"));

	}
	else
	{
		MIRALOG(Warning, TEXT("44"));

	}

	auto Bullet = GameMode->ProjectilePool.Get()->ProjectilePool->GetObject<AMIRAProjectile>();
	if (Bullet)
	{
		Bullet->Activate(true);
		//Projectile->Initialize(MuzzleLocation, ShootDirection, WeaponData->ProjectileSpeed, WeaponData->Damage);
	}
	/*
	AActor* Bullet = GameMode->ProjectilePool.Get()->GetObject();
	if (!Bullet)
	{
		MIRALOG(Error, TEXT("Failed to get Bullet from Pool"));
		return;
	}


	// set owner
	Bullet->Owner = this;

	auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (nullptr == TargetPlayer)	return;

	FVector BulletSpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;
	BulletSpawnLocation.Z += 50.0f;
	FVector BulletDir = GetActorForwardVector();

	Bullet->SetActorLocation(BulletSpawnLocation);
	UProjectileMovementComponent* ProjectileMovement = Bullet->FindComponentByClass<UProjectileMovementComponent>();
	if (ProjectileMovement)
	{
		FVector BulletVel = BulletDir * 2000.0f;
		ProjectileMovement->SetVelocityInLocalSpace(BulletVel);
	}
	*/
}

void AMIRAPlayerCharacter::UpDown(float NewAxisValue)
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

void AMIRAPlayerCharacter::LeftRight(float NewAxisValue)
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

void AMIRAPlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AMIRAPlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AMIRAPlayerCharacter::Block()
{
}

void AMIRAPlayerCharacter::Dodge()
{
}

void AMIRAPlayerCharacter::Execute()
{
}
