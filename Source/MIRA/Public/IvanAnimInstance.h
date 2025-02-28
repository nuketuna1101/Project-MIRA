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

    UAnimMontage* BasicAttackMontage;
    UAnimMontage* Skill1Montage;
    UAnimMontage* Skill2Montage;
    UAnimMontage* Skill3Montage;
    UAnimMontage* HitMontage;
    UAnimMontage* StunnedMontage;
    UAnimMontage* DeadMontage;
    UAnimSequence* IdleSequence;
    UAnimSequence* MoveSequence;

    void ChangeState(EBossState NewState);
    void UpdateState(float DeltaSeconds);
    void PlayAnimation();
};
