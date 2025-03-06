// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "MIRAAIController.h"
#include "MIRABaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);


	auto ControllingCharacter = Cast<AMIRABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingCharacter)		return EBTNodeResult::Failed;

	auto Target = Cast<AMIRABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMIRAAIController::TargetActorKey));
	if (nullptr == Target)	return EBTNodeResult::Failed;

	FVector DirectionVector = Target->GetActorLocation() - ControllingCharacter->GetActorLocation();
	DirectionVector.Z = 0.0f;

	FRotator TargetRot = FRotationMatrix::MakeFromX(DirectionVector).Rotator();

	ControllingCharacter->SetActorRotation(
		FMath::RInterpTo(ControllingCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));

	return EBTNodeResult::Succeeded;
}
