// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABlade.h"
#include "GameFramework/Character.h"
#include "MIRACharacter.generated.h"

// declare delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEvent, FVector, HitLocation);		// if hit, get location and timing 

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

	// damage framework
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
		AController* EventInstigator, AActor* DamageCauser) override;

	// camera components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* RightBlade;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AMIRABlade* LeftBlade;

	// getter for weapon mesh
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeRight();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AMIRABlade* GetBladeLeft();

	// attack action
	void Attack();

	void StartAim();

	void StopAim();


	// delegate for when attack hit
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAttackEnd OnAttackEndBP;

	// delegate for when be hitted
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHitEvent OnHitBP;

private:
	// basic movements by axis mapping
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	// player input actions by action mapping
	void Dodge();

	// attack logics
	void PerformAttackCombo();
	void SaveAttackCombo();
	void ResetAttackCombo();

	void AttackCheck();

	UPROPERTY()
	class UMIRAAnimInstance* MIRAAnim;

	// boolean for blocking or allowing moves
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bCannotMove;


	// boolean for dodge
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Dodge", Meta = (AllowPrivateAccess = true))
	bool bIsDodgeMode;

	// attack combo variables
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bSaveAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentComboCount;

	// aim variable
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bIsAiming;
};
