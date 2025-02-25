// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPlayerCameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
/*
// Sets default values for this component's properties
UMIRAPlayerCameraComponent::UMIRAPlayerCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMIRAPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter: %s"), *OwnerCharacter->GetName());
    UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter Class: %s"), *OwnerCharacter->GetClass()->GetName());

    if (OwnerCharacter)
    {
        SpringArm = NewObject<USpringArmComponent>(OwnerCharacter, TEXT("SpringArm"));
        Camera = NewObject<UCameraComponent>(OwnerCharacter, TEXT("Camera"));

        if (SpringArm && Camera)
        {
            UE_LOG(LogTemp, Warning, TEXT("TargetArmLength1: %f"), SpringArm->TargetArmLength);

            SpringArm->SetupAttachment(OwnerCharacter->GetCapsuleComponent());
            Camera->SetupAttachment(SpringArm);
            UE_LOG(LogTemp, Warning, TEXT("TargetArmLength2: %f"), SpringArm->TargetArmLength);

            SpringArm->TargetArmLength = 450.0f;
        }
    }
}

// Called every frame
void UMIRAPlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

    if (SpringArm)
    {
        //SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, SpringArmLength, DeltaTime, SpringArmLengthSpeed);

    }
}

void UMIRAPlayerCameraComponent::SetCameraMode(ECameraMode CameraMode)
{
	CurrentControlMode = CameraMode;
	if (!(SpringArm && Camera)) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	switch (CurrentControlMode)
	{
	case ECameraMode::FreeTPS:
        SpringArm->TargetArmLength = 450.0f;
        SpringArm->bUsePawnControlRotation = true;
        SpringArm->bInheritPitch = true;
        SpringArm->bInheritRoll = true;
        SpringArm->bInheritYaw = true;
        SpringArm->bDoCollisionTest = true;
        OwnerCharacter->bUseControllerRotationYaw = false;
        OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
        OwnerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
        OwnerCharacter->GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
        break;
	}
}
*/