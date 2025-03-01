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

	// play montages for proper action
	void PlayAttackComboMontage(int32 CurrentComboCount);
	void PlayDashMontage();
	void PlayDodgeMontage();
	void PlayExecuteMontage();
	void PlayHitMontage();
	void PlayStunnedMontage();

	void JumpToAttackMontageSection(int32 NewSection);

	// delegates
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnSaveAttackCheckDelegate OnSaveAttackCheck;
	FOnResetComboCheckDelegate OnResetComboCheck;

	void SetDeadAnim() { IsDead = true; }

private:

#pragma region Anim Notifies
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_SaveAttackCheck();

	UFUNCTION()
	void AnimNotify_ResetComboCheck();
#pragma endregion

	FName GetAttackMontageSectionName(int32 Section);



	// variable for basic movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsDead;

#pragma region Anim Montage assets
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* DashMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* DodgeMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* ExecuteMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* GetHitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* StunnedMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageA;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageB;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageC;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageD;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackComboMontageE;
#pragma endregion

};
