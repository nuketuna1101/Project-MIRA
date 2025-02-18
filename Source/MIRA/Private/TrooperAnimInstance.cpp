// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperAnimInstance.h"

UTrooperAnimInstance::UTrooperAnimInstance()
{
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
		DEAD_MONTAGE(TEXT(""));
	if (DEAD_MONTAGE.Succeeded())
	{
		DeadMontage = DEAD_MONTAGE.Object;
	}
}

void UTrooperAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// get pawn speed from pawn
	auto Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn)) return;



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

void UTrooperAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		//MIRALOG(Warning, TEXT("PlayAttackMontage"));
		Montage_Play(AttackMontage, 1.0f);
		//Montage_JumpToSection(FName("Attack"), AttackMontage);
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

