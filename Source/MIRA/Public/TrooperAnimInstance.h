// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//
	void PlayAttackMontage();
	void PlayHitMontage();
	void PlayDeadMontage();

	void SetDeadAnim() { IsDead = true; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsStrafing;

private:
	// anim notify

	// variable for basic movement
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeadMontage;
};
