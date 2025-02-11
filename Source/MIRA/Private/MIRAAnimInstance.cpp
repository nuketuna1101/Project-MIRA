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
		ATTACK_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MPlayerAnimMontage.MPlayerAnimMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// set montage : dodge
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DODGE_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MPlayerDodgeMontage.MPlayerDodgeMontage"));
	if (DODGE_MONTAGE.Succeeded())
	{
		DodgeMontage = DODGE_MONTAGE.Object;
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

void UMIRAAnimInstance::PlayAttackMontage()
{
	//Montage_Play(AttackMontage, 1.0f);

	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

void UMIRAAnimInstance::PlayDodgeMontage()
{
	if (!Montage_IsPlaying(DodgeMontage))
	{
		Montage_Play(DodgeMontage, 2.0f);
	}
}

void UMIRAAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UMIRAAnimInstance::AnimNotify_AttackHitCheck()
{
	// delegate broadcast
	OnAttackHitCheck.Broadcast();
}

void UMIRAAnimInstance::AnimNotify_NextAttackCheck()
{
	// delegate broadcast
	OnNextAttackCheck.Broadcast();
}

FName UMIRAAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack %d"), Section));
}
