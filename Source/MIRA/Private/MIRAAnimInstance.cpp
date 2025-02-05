// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAAnimInstance.h"

UMIRAAnimInstance::UMIRAAnimInstance()
{
	// pawn speed
	CurrentPawnSpeed = 0.0f;
	// is in air?
	IsInAir = false;
}

void UMIRAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
