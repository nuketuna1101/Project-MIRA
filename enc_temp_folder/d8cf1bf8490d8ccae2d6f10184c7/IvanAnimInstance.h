// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Animation/AnimInstance.h"
#include "IvanAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
    Idle,
    Move,
    BasicAttack,
    Skill1,
    Skill2,
    Skill3,
    Hit,
    Stunned,
    Dead
};

/**
 * 
 */
UCLASS()
class MIRA_API UIvanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UIvanAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    EBossState CurrentState;
    float StateTimer;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimSequence* IdleSequence;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimSequence* MoveSequence;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* BasicAttackMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* Skill1Montage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* Skill2Montage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* Skill3Montage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* HitMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* StunnedMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* DeadMontage;

    void ChangeState(EBossState NewState);
    void UpdateState(float DeltaSeconds);
    void PlayAnimation();
};
