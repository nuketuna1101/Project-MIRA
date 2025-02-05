// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRACharacter.h"

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
		SK_Kallari(TEXT("/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari"));
	if (SK_Kallari.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Kallari.Object);
	}

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	//	SK_Wraith(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	//if (SK_Wraith.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_Wraith.Object);
	//}

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

// Called to bind functionality to input
void AMIRACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings for action mapping
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);


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
