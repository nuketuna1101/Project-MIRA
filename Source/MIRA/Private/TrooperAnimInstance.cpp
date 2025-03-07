// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperAnimInstance.h"

UTrooperAnimInstance::UTrooperAnimInstance()
{
	// owner setting
	OwnerTrooper = nullptr;

	// setting varaibles for basic movement
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsStrafing = false;
	IsDead = false;

	// set montage : attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooper/MarksmanTrooperAttackAnimMontage.MarksmanTrooperAttackAnimMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		HIT_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooper/MarksmanTrooperHitMontage.MarksmanTrooperHitMontage"));
	if (HIT_MONTAGE.Succeeded())
	{
		HitMontage = HIT_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DEAD_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooper/MarksmanTrooperDeadMontage.MarksmanTrooperDeadMontage"));
	if (DEAD_MONTAGE.Succeeded())
	{
		DeadMontage = DEAD_MONTAGE.Object;
	}
}

void UTrooperAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// get pawn speed from pawn
	auto Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn)) return;
	else
	{
		OwnerTrooper = Cast<AMIRAMarksmanTrooper>(Pawn);
	}
}

void UTrooperAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsDead)
	{
		if (!IsValid(OwnerTrooper)) return;

		CurrentPawnSpeed = OwnerTrooper->GetVelocity().Size();
		// character jump
		if (OwnerTrooper)
		{
			IsInAir = OwnerTrooper->GetMovementComponent()->IsFalling();
		}
	}
}

void UTrooperAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

void UTrooperAnimInstance::PlayHitMontage()
{
	if (!Montage_IsPlaying(HitMontage))
	{
		Montage_Play(HitMontage, 1.0f);
	}
}

void UTrooperAnimInstance::PlayDeadMontage()
{
	if (!Montage_IsPlaying(DeadMontage))
	{
		Montage_Play(DeadMontage, 1.0f);
	}
}

void UTrooperAnimInstance::AnimNotify_MMFireStart()
{
	OwnerTrooper->FireProjectile();
}

void UTrooperAnimInstance::AnimNotify_MMFireEnd()
{
	OwnerTrooper->IsAttacking = false;
}