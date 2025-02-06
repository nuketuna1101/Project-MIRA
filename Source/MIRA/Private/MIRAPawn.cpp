// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPawn.h"

// MIRAPawn Constructor: Sets default values
AMIRAPawn::AMIRAPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create default subobjects
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	// hierarchy
	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	// transform setting
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		SK_Wraith(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (SK_Wraith.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_Wraith.Object);
	}


	// setting for animations
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> 
		MIRAPLAYER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MPlayerAnimBlueprint.MPlayerAnimBlueprint_C"));
	if (MIRAPLAYER_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(MIRAPLAYER_ANIM.Class);
	}

}

// Called when the game starts or when spawned
void AMIRAPawn::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	// setting for animations
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Animations/Jog_Fwd_Start.Jog_Fwd_Start"));
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
	*/
}

// Called every frame
void AMIRAPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMIRAPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// bindings
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMIRAPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMIRAPawn::LeftRight);

}

void AMIRAPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMIRAPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMIRAPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AMIRAPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}

