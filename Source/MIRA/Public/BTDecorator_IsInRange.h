// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API UBTDecorator_IsInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
