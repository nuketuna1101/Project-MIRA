// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusTarget.h"
#include "TrooperAIController.h"
#include "MIRAEnemyBaseCharacter.h"
#include "MIRACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_FocusTarget::UBTTask_FocusTarget()
{
	NodeName = TEXT("Focus Target");
	bNotifyTick = true;

	// time setting
	FocusTime = 3.0f;
	ElapsedTime = 0.0f;
}

EBTNodeResult::Type UBTTask_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BBComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BBComp)	return EBTNodeResult::Failed;

	auto Trooper = Cast<AMIRAEnemyBaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Trooper)	return EBTNodeResult::Failed;

	auto Target = Cast<AMIRACharacter>(BBComp->GetValueAsObject(ATrooperAIController::TargetKey));
	if (nullptr == Target)	return EBTNodeResult::Failed;

	auto TrooperLocation = Trooper->GetActorLocation();
	auto TargetLocation = Target->GetActorLocation();
	auto TrooperRotation = Trooper->GetActorRotation();

	FVector FocusDir = TargetLocation - TrooperLocation;
	FRotator FocusRot = FRotationMatrix::MakeFromX(FocusDir).Rotator();

	Trooper->SetActorRotation(FMath::RInterpTo(TrooperRotation, FocusRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	FVector StrafeDir = Trooper->GetActorRightVector();
	Trooper->GetCharacterMovement()->Velocity = StrafeDir * 300.0f;

	ElapsedTime = 0.0f;

	return EBTNodeResult::InProgress;
}

void UBTTask_FocusTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ElapsedTime += DeltaSeconds;

	if (DeltaSeconds >= FocusTime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
