// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAAnimInstance.h"

UMIRAAnimInstance::UMIRAAnimInstance()
{
	// setting varaibles for basic movement
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// set montage : attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DASH_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerDashMontage.MPlayerDashMontage"));
	if (DASH_MONTAGE.Succeeded())
	{
		DashMontage = DASH_MONTAGE.Object;
	}

	// set montage : dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DODGE_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerDodgeMontage.MPlayerDodgeMontage"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
	}

	// set montage : execute
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		EXECUTE_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerShadowStrikeMontage.MPlayerShadowStrikeMontage"));
	if (EXECUTE_MONTAGE.Succeeded())
	{
		ExecuteMontage = EXECUTE_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		GETHIT_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MIRAPlayerHitMontage.MIRAPlayerHitMontage"));
	if (GETHIT_MONTAGE.Succeeded())
	{
		GetHitMontage = GETHIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		STUNNED_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerStunnedMontage.MPlayerStunnedMontage"));
	if (STUNNED_MONTAGE.Succeeded())
	{
		StunnedMontage = STUNNED_MONTAGE.Object;
	}



	// set montage : attack combo
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACKCOMBO_MONTAGE_A(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAttackMontage_A.MPlayerAttackMontage_A"));
	if (ATTACKCOMBO_MONTAGE_A.Succeeded())
	{
		AttackComboMontageA = ATTACKCOMBO_MONTAGE_A.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACKCOMBO_MONTAGE_B(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAttackMontage_B.MPlayerAttackMontage_B"));
	if (ATTACKCOMBO_MONTAGE_B.Succeeded())
	{
		AttackComboMontageB = ATTACKCOMBO_MONTAGE_B.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACKCOMBO_MONTAGE_C(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAttackMontage_C.MPlayerAttackMontage_C"));
	if (ATTACKCOMBO_MONTAGE_C.Succeeded())
	{
		AttackComboMontageC = ATTACKCOMBO_MONTAGE_C.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACKCOMBO_MONTAGE_D(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAttackMontage_D.MPlayerAttackMontage_D"));
	if (ATTACKCOMBO_MONTAGE_D.Succeeded())
	{
		AttackComboMontageD = ATTACKCOMBO_MONTAGE_D.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACKCOMBO_MONTAGE_E(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerAttackMontage_E.MPlayerAttackMontage_E"));
	if (ATTACKCOMBO_MONTAGE_E.Succeeded())
	{
		AttackComboMontageE = ATTACKCOMBO_MONTAGE_E.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RANGEDATTACK_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MIRAPlayer/MPlayerRangedAttackMontage.MPlayerRangedAttackMontage"));
	if (RANGEDATTACK_MONTAGE.Succeeded())
	{
		RangedAttackMontage = RANGEDATTACK_MONTAGE.Object;
	}
}

void UMIRAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// get pawn speed from pawn
	auto Pawn = TryGetPawnOwner();

	if (!IsValid(Pawn))	return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// character jump
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMIRAAnimInstance::PlayAttackComboMontage(int32 CurrentComboCount)
{
	//MIRALOG(Warning, TEXT("[PlayAttackComboMontage] CurrentComboCount: %d"), CurrentComboCount);
	switch (CurrentComboCount)
	{
	case 0:
		Montage_Play(AttackComboMontageA, 1.0f);
		break;
	case 1:
		Montage_Play(AttackComboMontageB, 1.0f);
		break;
	case 2:
		Montage_Play(AttackComboMontageC, 1.0f);
		break;
	case 3:
		Montage_Play(AttackComboMontageD, 1.0f);
		break;
	case 4:
		Montage_Play(AttackComboMontageE, 1.0f);
		break;
	}
}

void UMIRAAnimInstance::PlayDashMontage()
{
	if (!Montage_IsPlaying(DashMontage))
	{
		Montage_Play(DashMontage, 4.0f);
	}
}

void UMIRAAnimInstance::PlayDodgeMontage()
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		Montage_Play(DodgeMontage, 1.0f);
	}
}

void UMIRAAnimInstance::PlayExecuteMontage()
{
	if (!Montage_IsPlaying(ExecuteMontage))
	{
		Montage_Play(ExecuteMontage, 1.0f);
	}
}

void UMIRAAnimInstance::PlayHitMontage()
{
	MIRALOG(Warning, TEXT("UMIRAAnimInstance::PlayHitMontage called"));
	if (!Montage_IsPlaying(GetHitMontage))
	{
		MIRALOG(Warning, TEXT("Montage_Play leggo called"));
		Montage_Play(GetHitMontage, 1.0f);
	}
}

void UMIRAAnimInstance::PlayStunnedMontage()
{
	if (!Montage_IsPlaying(StunnedMontage))
	{
		Montage_Play(StunnedMontage, 1.0f);
	}
}

void UMIRAAnimInstance::PlayRangedAttackMontage()
{
	if (!Montage_IsPlaying(RangedAttackMontage))
	{
		Montage_Play(RangedAttackMontage, 1.0f);
	}
}

void UMIRAAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//
	//Montage_JumpToSection(GetAttackMontageSectionName(NewSection), );
}

void UMIRAAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UMIRAAnimInstance::AnimNotify_SaveAttackCheck()
{
	OnSaveAttackCheck.Broadcast();
}

void UMIRAAnimInstance::AnimNotify_ResetComboCheck()
{
	OnResetComboCheck.Broadcast();
}

void UMIRAAnimInstance::AnimNotify_ThrowRanged()
{
	MIRALOG(Warning, TEXT("AnimNotify_ThrowRanged"));
	OnThrowRangedCheck.Broadcast();
}

FName UMIRAAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack %d"), Section));
}
