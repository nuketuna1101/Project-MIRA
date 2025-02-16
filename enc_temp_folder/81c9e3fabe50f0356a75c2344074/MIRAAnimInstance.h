// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Animation/AnimInstance.h"
#include "MIRAAnimInstance.generated.h"

// delegate
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSaveAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnResetComboCheckDelegate);

/**
 * 
 */
UCLASS()
class MIRA_API UMIRAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMIRAAnimInstance();

	// 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//
	void PlayAttackMontage();
	void PlayAttackComboMontage(int32 CurrentComboCount);
	void PlayDodgeMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	// delegates
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnSaveAttackCheckDelegate OnSaveAttackCheck;
	FOnResetComboCheckDelegate OnResetComboCheck;

	void SetDeadAnim() { IsDead = true; }

private:
	// anim notify
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_SaveAttackCheck();

	UFUNCTION()
	void AnimNotify_ResetComboCheck();

	FName GetAttackMontageSectionName(int32 Section);



	// variable for basic movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;




	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageA;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageB;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageC;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageD;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageE;
};
