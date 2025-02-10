// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperAnimInstance.h"

UTrooperAnimInstance::UTrooperAnimInstance()
{
	// setting varaibles for basic movement
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;

	// set montage : attack
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooperAttackAnimMontage.MarksmanTrooperAttackAnimMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UTrooperAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// get pawn speed from pawn
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
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
	Montage_Play(AttackMontage, 1.0f);

	//if (!AttackMontage)
	//{
	//	MIRALOG(Error, TEXT("AttackMontage is nullptr!"));
	//	return;
	//}

	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	MIRALOG(Warning, TEXT("PlayAttackMontage"));
	//	Montage_Play(AttackMontage, 1.0f);
	//	Montage_JumpToSection(FName("Attack"), AttackMontage);
	//}
	//else
	//{
	//	MIRALOG(Warning, TEXT("Attack Montage is already playing"));
	//}

	//// 애니메이션이 재생 중인지를 확인하는 로그 추가
	//if (Montage_IsPlaying(AttackMontage))
	//{
	//	MIRALOG(Warning, TEXT("Attack Montage is now playing!"));
	//}


	if (!Montage_IsPlaying(AttackMontage))
	{
		MIRALOG(Warning, TEXT("PlayAttackMontage"));
		Montage_Play(AttackMontage, 1.0f);
		Montage_JumpToSection(FName("Attack"), AttackMontage);
	}
}

