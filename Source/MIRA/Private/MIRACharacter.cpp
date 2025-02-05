// Fill out your copyright notice in the Description page of Project Settings.

#include "MIRACharacter.h"
#include "MIRAAnimInstance.h"

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
		MIRAPLAYER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MPlayerAnimBlueprint.MPlayerAnimBlueprint_C"));
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
	IsAttacking = false;

	// attack combo
	MaxCombo = 2;
	AttackEndComboState();
}

// Called when the game starts or when spawned
void AMIRACharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	MIRAAnim = Cast<UMIRAAnimInstance>(GetMesh()->GetAnimInstance());
	MIRACHECK(nullptr != MIRAAnim);

	// event binding on montage end
	if (!MIRAAnim) return;
	MIRAAnim->OnMontageEnded.AddDynamic(this, &AMIRACharacter::OnAttackMontageEnded);

	// when nextattackcheck, delegate execution
	MIRAAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		MIRALOG(Warning, TEXT("[OnNextAttackCheck]"));
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartComboState();
			MIRAAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
}

// Called to bind functionality to input
void AMIRACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings for action mapping
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMIRACharacter::Attack);


	// bindings for axis mapping
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMIRACharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMIRACharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMIRACharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMIRACharacter::LookUp);
}

void AMIRACharacter::UpDown(float NewAxisValue)
{
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
}

void AMIRACharacter::LeftRight(float NewAxisValue)
{
	//AddMovementInput(GetActorRightVector(), NewAxisValue);
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

void AMIRACharacter::Attack()
{
	if (IsAttacking)
	{
		//MIRACHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		MIRACHECK(CurrentCombo == 0);
		AttackStartComboState();
		MIRAAnim->PlayAttackMontage();
		MIRAAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
	/*
	// bool check
	if (IsAttacking) return;

	// basic attack
	MIRALOG(Warning, TEXT("[MIRACharacter] on Attack called"));

	// handling by attack montage in anim instance
	MIRAAnim->PlayAttackMontage();

	// set bool
	IsAttacking = true;
	*/
}

void AMIRACharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMIRACharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMIRACharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	MIRACHECK(IsAttacking);
	IsAttacking = false;
	AttackEndComboState();
}
