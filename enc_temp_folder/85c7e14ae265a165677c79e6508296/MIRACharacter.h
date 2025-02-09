// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Character.h"
#include "MIRACharacter.generated.h"

// declare delegates
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM()
enum class ECameraMode : uint8
{
	FreeTPS,
};

UCLASS()
class MIRA_API AMIRACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMIRACharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// camera mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ECameraMode CurrentControlMode = ECameraMode::FreeTPS;

	void SetCameraMode(ECameraMode CameraMode);

	FRotator SpringArmRotation = FRotator::ZeroRotator;
	float SpringArmLength = 0.0f;
	float SpringArmRotationSpeed = 0.0f;
	float SpringArmLengthSpeed = 0.0f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// camera components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	// attack action
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	// movement logics by player input 
	// basic movement
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	// attack action
	void AttackStartComboState();
	void AttackEndComboState();

	// montage
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY()
	class UMIRAAnimInstance* MIRAAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
};
