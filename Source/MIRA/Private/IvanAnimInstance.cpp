// Fill out your copyright notice in the Description page of Project Settings.

#include "IvanAnimInstance.h"

UIvanAnimInstance::UIvanAnimInstance()
{
    OwnerIvan = nullptr;

	// asset loading
	CurrentState = EBossState::Idle;
	StateTimer = 0.0f;

    // set montage : attack
    static ConstructorHelpers::FObjectFinder<UAnimMontage>
        PATTERN_A_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/Ivan/IvanPatternAMontage.IvanPatternAMontage"));
    if (PATTERN_A_MONTAGE.Succeeded())
    {
        PatternAMontage = PATTERN_A_MONTAGE.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage>
        PATTERN_B_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/Ivan/IvanPatternBMontage.IvanPatternBMontage"));
    if (PATTERN_B_MONTAGE.Succeeded())
    {
        PatternBMontage = PATTERN_B_MONTAGE.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage>
        PATTERN_C_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/Ivan/IvanPatternCMontage.IvanPatternCMontage"));
    if (PATTERN_C_MONTAGE.Succeeded())
    {
        PatternCMontage = PATTERN_C_MONTAGE.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage>
        VANISH_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/Ivan/IvanVanishMontage.IvanVanishMontage"));
    if (VANISH_MONTAGE.Succeeded())
    {
        VanishMontage = VANISH_MONTAGE.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage>
        EMOTE_A_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/Ivan/IvanStareEmoteMontage.IvanStareEmoteMontage"));
    if (EMOTE_A_MONTAGE.Succeeded())
    {
        EmoteAMontage = EMOTE_A_MONTAGE.Object;
    }
}

void UIvanAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    CurrentState = EBossState::Idle;
    StateTimer = 0.0f;

    auto Pawn = TryGetPawnOwner();
    if (!IsValid(Pawn)) return;
    else
    {
        OwnerIvan = Cast<AMIRABossIvan>(Pawn);
    }
}

void UIvanAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    UpdateState(DeltaSeconds);
}

void UIvanAnimInstance::PlayMontage(EBossState NewState)
{
    switch (NewState)
    {
    case EBossState::PatternA:
        Montage_Play(PatternAMontage);
        break;
    case EBossState::PatternB:
        Montage_Play(PatternBMontage);
        break;
    case EBossState::PatternC:
        Montage_Play(PatternCMontage);
        break;
    case EBossState::Vanish:
        Montage_Play(VanishMontage);
        break;
    default:
        Montage_Play(PatternCMontage);
        break;
    }
}

void UIvanAnimInstance::AnimNotify_IvanFireHoming()
{
    OnIvanFireHomingBP.Broadcast();
    if (OwnerIvan) OwnerIvan->FireHomings();
}

void UIvanAnimInstance::AnimNotify_IvanThrowGB()
{
    OnIvanThrowGBBP.Broadcast();
    if (OwnerIvan) OwnerIvan->ThrowBoomGrounds();
}

void UIvanAnimInstance::AnimNotify_IvanLaunchGB()
{
    OnIvanLaunchGBBP.Broadcast();
    if (OwnerIvan) OwnerIvan->LaunchBoomGrounds();
}

void UIvanAnimInstance::AnimNotify_IvanCastDS()
{
    OnIvanCastDSBP.Broadcast();
    if (OwnerIvan) OwnerIvan->CastDeathStare();
}

void UIvanAnimInstance::AnimNotify_IvanDeathStare()
{
    OnIvanDeathStareBP.Broadcast();
    if (OwnerIvan) OwnerIvan->DeathStare();
}

void UIvanAnimInstance::AnimNotify_IvanStartVanish()
{
    OnIvanStartVanishBP.Broadcast();
    if (OwnerIvan) OwnerIvan->StartVanish();
}

void UIvanAnimInstance::AnimNotify_IvanEndVanish()
{
    OnIvanEndVanishBP.Broadcast();
    if (OwnerIvan) OwnerIvan->EndVanish();
}

void UIvanAnimInstance::ChangeState(EBossState NewState)
{
	if (CurrentState != NewState)
	{
        FString CurrentStateString;
        switch (CurrentState)
        {
        case EBossState::Idle:
            CurrentStateString = TEXT("Idle");
            break;
        case EBossState::PatternA:
            CurrentStateString = TEXT("PatternA");
            break;
        case EBossState::PatternB:
            CurrentStateString = TEXT("PatternB");
            break;
        case EBossState::PatternC:
            CurrentStateString = TEXT("PatternC");
            break;
        case EBossState::Vanish:
            CurrentStateString = TEXT("Vanish");
            break;
        default:
            CurrentStateString = TEXT("Unknown");
            break;
        }

        FString NewStateString;
        switch (NewState)
        {
        case EBossState::Idle:
            NewStateString = TEXT("Idle");
            break;
        case EBossState::PatternA:
            NewStateString = TEXT("PatternA");
            break;
        case EBossState::PatternB:
            NewStateString = TEXT("PatternB");
            break;
        case EBossState::PatternC:
            NewStateString = TEXT("PatternC");
            break;
        case EBossState::Vanish:
            NewStateString = TEXT("Vanish");
            break;
        default:
            NewStateString = TEXT("Unknown");
            break;
        }

        MIRALOG(Warning, TEXT("State Changed: %s -> %s"), *CurrentStateString, *NewStateString);
        CurrentState = NewState;
		StateTimer = 0.0f;
        PlayAnimation();
	}
}

void UIvanAnimInstance::UpdateState(float DeltaSeconds)
{
	StateTimer += DeltaSeconds;

    // state converting logics
    switch (CurrentState)
    {
    case EBossState::Idle:
        if (StateTimer > 5.0f && Montage_GetIsStopped(EmoteAMontage))
        {
            MIRALOG(Warning, TEXT("StateTimer: %f, MontageStopped: %d"), StateTimer, Montage_GetIsStopped(EmoteAMontage));
            int32 SkillIndex = FMath::RandRange(1, 4);
            MIRALOG(Warning, TEXT("SkillIndex: %d"), SkillIndex);
            if (SkillIndex == 1) ChangeState(EBossState::PatternA);
            else if (SkillIndex == 2) ChangeState(EBossState::PatternB);
            else if (SkillIndex == 3) ChangeState(EBossState::PatternC);
            else if (SkillIndex == 4) ChangeState(EBossState::Vanish);
        }
        break;
    case EBossState::PatternA:
        if (Montage_GetIsStopped(PatternAMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::PatternB:
        if (Montage_GetIsStopped(PatternBMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::PatternC:
        if (Montage_GetIsStopped(PatternCMontage))
        {
            ChangeState(EBossState::Idle);
        }
        break;
    case EBossState::Vanish:
        if (Montage_GetIsStopped(VanishMontage))
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
        Montage_Play(EmoteAMontage);
        break;
    case EBossState::PatternA:
        Montage_Play(PatternAMontage);
        break;
    case EBossState::PatternB:
        Montage_Play(PatternBMontage);
        break;
    case EBossState::PatternC:
        Montage_Play(PatternCMontage);
        break;
    case EBossState::Vanish:
        Montage_Play(VanishMontage);
        break;
    case EBossState::Dead:
        //Montage_Play(DeadMontage);
        break;
    }
}
