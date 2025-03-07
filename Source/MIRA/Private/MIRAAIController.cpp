// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#pragma region Definition for BB Keys
const FName AMIRAAIController::InitLocKey(TEXT("InitLocKey"));
const FName AMIRAAIController::TargetActorKey(TEXT("TargetActorKey"));
const FName AMIRAAIController::TargetDistanceKey(TEXT("TargetDistanceKey"));
#pragma endregion

AMIRAAIController::AMIRAAIController()
{
	// setting for BB and BB assets
	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		BBObject(TEXT("/Game/MIRA/Characters/AI/BB_Trooper.BB_Trooper"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BTObject(TEXT("/Game/MIRA/Characters/AI/BT_Trooper.BT_Trooper"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AMIRAAIController::SetAIAssets(UBlackboardData* NewBBAsset, UBehaviorTree* NewBTAsset)
{
	BBAsset = NewBBAsset;
	BTAsset = NewBTAsset;
}

void AMIRAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// init BB key setting and run BT
	auto BBComponent = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BBComponent))
	{
		BBComponent->SetValueAsVector(InitLocKey, InPawn->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			MIRALOG(Error, TEXT("CANNOT RUN BT"));
		}
	}
}

void AMIRAAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMIRAAIController::RunAI()
{
	MIRALOG(Warning, TEXT("[RUN AI]"));
	auto BBComp = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BBComp))
	{
		//Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			MIRALOG(Error, TEXT("cannot run BT"));
		}
	}
}

void AMIRAAIController::StopAI()
{
	MIRALOG(Warning, TEXT("[Stop AI]"));
	auto BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BTComp)
	{
		BTComp->StopTree(EBTStopMode::Safe);
	}
}
