// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInRange.h"
#include "MIRAAIController.h"
#include "MIRAPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInRange::UBTDecorator_IsInRange()
{
	NodeName = TEXT("IsAbleToAttack");
}

bool UBTDecorator_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bSuper = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return false;

	auto BBComp = OwnerComp.GetBlackboardComponent();
	auto Target = Cast<AMIRAPlayerCharacter>(BBComp->GetValueAsObject(AMIRAAIController::TargetActorKey));
	if (nullptr == Target) return false;

	bSuper = Target->GetDistanceTo(ControllingPawn) <= 600.0f;
	return bSuper;
}
