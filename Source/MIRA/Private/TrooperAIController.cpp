// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATrooperAIController::HomePosKey(TEXT("HomePosKey"));
const FName ATrooperAIController::TargetKey(TEXT("TargetKey"));

ATrooperAIController::ATrooperAIController()
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

void ATrooperAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto BBComponent = GetBlackboardComponent();
	if (UseBlackboard(BBAsset, BBComponent))
	{
		BBComponent->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			MIRALOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void ATrooperAIController::OnUnPossess()
{
	Super::OnUnPossess();
}