// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABossIvan.h"
#include "Animation/AnimInstance.h"
#include "IvanAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanFireHoming);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanThrowGB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanLaunchGB);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanCastDS);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanDeathStare);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanStartVanish);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIvanEndVanish);

UENUM(BlueprintType)
enum class EBossState : uint8
{
    Idle,
    PatternA,
    PatternB,
    PatternC,
    Vanish,
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

public:
    void PlayMontage(EBossState NewState);

    void SetDeadAnim() { IsDead = true; }

#pragma region delegates

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanFireHoming OnIvanFireHomingBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanThrowGB OnIvanThrowGBBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanLaunchGB OnIvanLaunchGBBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanCastDS OnIvanCastDSBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanDeathStare OnIvanDeathStareBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanStartVanish OnIvanStartVanishBP;
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnIvanEndVanish OnIvanEndVanishBP;

#pragma endregion


private:
    AMIRABossIvan* OwnerIvan;

#pragma region Anim Notifies
    UFUNCTION()
    void AnimNotify_IvanFireHoming();
    UFUNCTION()
    void AnimNotify_IvanThrowGB();
    UFUNCTION()
    void AnimNotify_IvanLaunchGB();
    UFUNCTION()
    void AnimNotify_IvanCastDS();
    UFUNCTION()
    void AnimNotify_IvanDeathStare();
    UFUNCTION()
    void AnimNotify_IvanStartVanish();
    UFUNCTION()
    void AnimNotify_IvanEndVanish();
#pragma endregion

#pragma region other variables
    // variable for basic movement
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
    float CurrentPawnSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
    bool IsInAir;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
    bool bIsVanished;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", meta = (AllowPrivateAccess = true))
    bool IsDead;
#pragma endregion

#pragma region Anim Montage assets

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* PatternAMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* PatternBMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* PatternCMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* VanishMontage;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    UAnimMontage* EmoteAMontage;

    //UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
    //UAnimMontage* DeadMontage;

#pragma endregion

#pragma region State for anim control
    EBossState CurrentState;
    float StateTimer;

    void ChangeState(EBossState NewState);
    void UpdateState(float DeltaSeconds);
    void PlayAnimation();
#pragma endregion


#pragma region Pattern Selection Algorithm

    const float WeightDecreaseRate = 2.0f;

    TMap<EBossState, float> PatternWeights;

    void InitPatternWeight();

    EBossState GetSelectedPattern();

    void UpdatePatternWeight(EBossState SelectedPattern);

#pragma endregion

};
