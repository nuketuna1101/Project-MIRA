// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRAMarksmanTrooper.h"
#include "Animation/AnimInstance.h"
#include "TrooperAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UTrooperAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTrooperAnimInstance();

	// 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//
	void PlayAttackMontage();
	void PlayHitMontage();
	void PlayDeadMontage();

	void SetDeadAnim() { IsDead = true; }


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsStrafing;

private:

	AMIRAMarksmanTrooper* OwnerTrooper;

#pragma region Anim Notifies
	UFUNCTION()
	void AnimNotify_MMFireStart();

	UFUNCTION()
	void AnimNotify_MMFireEnd();

#pragma endregion


	// variable for basic movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsDead;

#pragma region Anim Montage assets

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;
#pragma endregion

};
