// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABlade.h"
#include "MIRACharacterStatComponent.h"
#include "GameFramework/Character.h"
#include "MIRACharacter.generated.h"

// declare delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDash);
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

#pragma region Camera Setting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ECameraMode CurrentControlMode = ECameraMode::FreeTPS;

	void SetCameraMode(ECameraMode CameraMode);

	FRotator SpringArmRotation = FRotator::ZeroRotator;
	float SpringArmLength = 0.0f;
	float SpringArmRotationSpeed = 0.0f;
	float SpringArmLengthSpeed = 0.0f;
#pragma endregion

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

	// getter for bIsWalking
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	bool IsWalking();

	// getter for bIsDashing
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	bool IsDashing();

	// attack action
	void Attack();
	void StartBlock();
	void StopBlock();
	void StartAim();
	void StopAim();
	void StartDash();

	// delegate for when attack hit
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAttackEnd OnAttackEndBP;
	// delegate for when be hitted
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHitEvent OnHitBP;
	// delegate for when start dash
	FOnStartDash OnStartDashBP;

	UPROPERTY(VisibleAnywhere, Category = "Character Data")
	class UMIRACharacterStatComponent* CharacterStat;


private:

#pragma region Input Action by axis and action mappings
	// basic movements by axis mapping
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	// player input actions by action mapping
	void Block();
	void Dodge();
	void Execute();
#pragma endregion



	// attack logics
	void AttackMelee();
	void AttackRange();

	void PerformAttackCombo();
	void SaveAttackCombo();
	void ResetAttackCombo();

	void AttackCheck();

	UPROPERTY()
	class UMIRAAnimInstance* MIRAAnim;

#pragma region Dash Variable
	FVector DashDirection;
	float MaxDashDistance;
	float CurrentDashDistance;
	float DashSpeed;
#pragma endregion


#pragma region Booleans for State/Mode

	// boolean for blocking or allowing moves
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bCannotMove;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bIsBlocking;

	// boolean for dodge
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bIsDodging;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bIsDashing;

	// attack combo variables
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bSaveAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	int32 CurrentComboCount;

	// aim variable
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", Meta = (AllowPrivateAccess = true))
	bool bIsAiming;

	// walking variable
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player Movement", Meta = (AllowPrivateAccess = true))
	bool bIsWalking;

#pragma endregion

#pragma region CharacterStat

	UPROPERTY(EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float MaxMP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentMP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 TotalUP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 CurrentUP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float Power;

#pragma endregion


};