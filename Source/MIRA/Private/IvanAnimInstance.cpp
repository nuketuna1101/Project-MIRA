// Fill out your copyright notice in the Description page of Project Settings.


#include "IvanAnimInstance.h"

UIvanAnimInstance::UIvanAnimInstance()
{
	// asset loading

	CurrentState = EBossState::Idle;
	StateTimer = 0.0f;
}

void UIvanAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UIvanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateState(DeltaSeconds);
	PlayAnimation();
}

void UIvanAnimInstance::ChangeState(EBossState NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
		StateTimer = 0.0f;
	}
}

void UIvanAnimInstance::UpdateState(float DeltaSeconds)
{
	StateTimer += DeltaSeconds;

    // state converting logics
    switch (CurrentState)
    {
    case EBossState::Idle:
        if (StateTimer > 3.0f)
        {
            ChangeState(EBossState::Move);
        }
        break;
    case EBossState::Move:
        if (FMath::FRand() < 0.1f)
        {
            int32 SkillIndex = FMath::RandRange(0, 3);
            if (SkillIndex == 0) ChangeState(EBossState::BasicAttack);
            else if (SkillIndex == 1) ChangeState(EBossState::Skill1);
            else if (SkillIndex == 2) ChangeState(EBossState::Skill2);
            else if (SkillIndex == 3) ChangeState(EBossState::Skill3);
        }
        break;
    case EBossState::BasicAttack:
        if (Montage_GetIsStopped(BasicAttackMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Skill1:
        if (Montage_GetIsStopped(Skill1Montage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Skill2:
        if (Montage_GetIsStopped(Skill2Montage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Skill3:
        if (Montage_GetIsStopped(Skill3Montage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Hit:
        if (Montage_GetIsStopped(HitMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Stunned:
        if (Montage_GetIsStopped(StunnedMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Dead:
        //
        break;
    }
}

void UIvanAnimInstance::PlayAnimation()
{
    switch (CurrentState)
    {
    case EBossState::Idle:
        Montage_Stop(0.0f);
        PlaySlotAnimationAsDynamicMontage(IdleSequence, FName("DefaultSlot"), 0.2f, 0.2f);
        break;
    case EBossState::Move:
        Montage_Stop(0.0f);
        PlaySlotAnimationAsDynamicMontage(MoveSequence, FName("DefaultSlot"), 0.2f, 0.2f);
        break;
    case EBossState::BasicAttack:
        Montage_Play(BasicAttackMontage);
        break;
    case EBossState::Skill1:
        Montage_Play(Skill1Montage);
        break;
    case EBossState::Skill2:
        Montage_Play(Skill2Montage);
        break;
    case EBossState::Skill3:
        Montage_Play(Skill3Montage);
        break;
    case EBossState::Hit:
        Montage_Play(HitMontage);
        break;
    case EBossState::Stunned:
        Montage_Play(StunnedMontage);
        break;
    case EBossState::Dead:
        Montage_Play(DeadMontage);
        break;
    }
}
