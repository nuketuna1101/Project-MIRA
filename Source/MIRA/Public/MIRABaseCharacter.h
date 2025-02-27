// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRAGameInstance.h"
#include "MIRACharacterStatComponent.h"
#include "GameFramework/Character.h"
#include "MIRABaseCharacter.generated.h"

// declare delegates

DECLARE_MULTICAST_DELEGATE(FOnAttackEndTrooper);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartDashDelegate, FVector, DashEfxLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEventDelegate, FVector, HitLocation);		// if hit, get location and timing 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadEventDelegate, AActor*, DamageCauser);


UCLASS()
class MIRA_API AMIRABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	AMIRABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

#pragma region CharacterState

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const { return ECharacterState(); }

#pragma endregion

#pragma region CharacterStat
	UPROPERTY(VisibleAnywhere, Category = "Character Data")
	class UMIRACharacterStatComponent* CharacterStat;
#pragma endregion

#pragma region [TO DO] Actions
	virtual void Attack();
	//void StartBlock();
	//void StopBlock();
	//void StartAim();
	//void StopAim();
	//void StartDash();
#pragma endregion

#pragma region [TO DO] Delegates for BP event
	FOnAttackEndTrooper OnAttackEnd;

	// delegate for when attack hit
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnAttackEndDelegate OnAttackEndBP;
	// delegate for when be hitted
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHitEventDelegate OnHitBP;
	// delegate for when start dash
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnStartDashDelegate OnStartDashBP;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnDeadEventDelegate OnDead;
#pragma endregion

#pragma region Asset Async Loading by Streamable Manager
	void OnAssetLoadCompleted();
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
#pragma endregion

protected:
	virtual void BeginPlay() override;

#pragma region [TO DO] Attack Logics
	//// attack logics
	//void AttackMelee();
	//void AttackRange();
	//void PerformAttackCombo();
	//void SaveAttackCombo();
	//void ResetAttackCombo();
	//void AttackCheck();
#pragma endregion

#pragma region Anim Instance
	UPROPERTY()
	class UMIRAAnimInstance* MIRAAnim;
#pragma endregion

#pragma region Dash Variable
	FVector DashDirection;
	float MaxDashDistance;
	float CurrentDashDistance;
	float DashSpeed;
#pragma endregion

#pragma region Booleans for State/Mode

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

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

#pragma region State and PlayerController

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY()
	class AMIRAPlayerController* MIRAPlayerController;

#pragma endregion

#pragma region Dead Timer

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;
	FTimerHandle DeadTimerHandle = {};
#pragma endregion

};
