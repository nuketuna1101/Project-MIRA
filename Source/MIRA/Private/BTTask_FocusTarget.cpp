// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusTarget.h"
#include "TrooperAIController.h"
#include "MIRABaseCharacter.h"
#include "MIRAPlayerCharacter.h"
#include "TrooperAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_FocusTarget::UBTTask_FocusTarget()
{
	NodeName = TEXT("Focus Target");
	bNotifyTick = true;

	// time setting
	FocusTime = 2.0f;
	ElapsedTime = 0.0f;
}

EBTNodeResult::Type UBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BBComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BBComp)	return EBTNodeResult::Failed;

	auto Trooper = Cast<AMIRABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return EBTNodeResult::Failed;

	auto Target = Cast<AMIRAPlayerCharacter>(BBComp->GetValueAsObject(ATrooperAIController::TargetKey));
	if (nullptr == Target)	return EBTNodeResult::Failed;

	ElapsedTime = 0.0f;

	LeftRight = FMath::Rand();
	RandX = FMath::FRandRange(-5.0f, 5.0f);
	RandY = FMath::FRandRange(-5.0f, 5.0f);

	// 
	auto AnimInstance = Cast<UTrooperAnimInstance>(Trooper->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->IsStrafing = true;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_FocusTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	auto BBComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BBComp)	return;

	auto Trooper = Cast<AMIRABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return;

	auto Target = Cast<AMIRAPlayerCharacter>(BBComp->GetValueAsObject(ATrooperAIController::TargetKey));
	if (nullptr == Target)	return;

	auto TrooperLocation = Trooper->GetActorLocation();
	auto TargetLocation = Target->GetActorLocation();
	auto TrooperRotation = Trooper->GetActorRotation();

	FVector FocusDir = TargetLocation - TrooperLocation;
	FRotator FocusRot = FRotationMatrix::MakeFromX(FocusDir).Rotator();

	Trooper->SetActorRotation(FMath::RInterpTo(TrooperRotation, FocusRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	FVector BaseDir = Trooper->GetActorRightVector();

	FVector StrafeDir = (LeftRight > 0.5f ? 1.0f : -1.0f) * BaseDir + FVector(RandX, RandY, 0.0f);
	Trooper->GetCharacterMovement()->Velocity = StrafeDir * 100.0f;

	ElapsedTime += DeltaSeconds;

	if (ElapsedTime >= FocusTime)
	{
		Trooper->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		auto AnimInstance = Cast<UTrooperAnimInstance>(Trooper->GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->IsStrafing = false;
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
