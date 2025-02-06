// Fill out your copyright notice in the Description page of Project Settings.


#include "TrooperAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

ATrooperAIController::ATrooperAIController()
{
	RepeatInterval = 3.0f;

}

void ATrooperAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ATrooperAIController::OnRepeatTimer, RepeatInterval, true);
}

void ATrooperAIController::OnUnPossess()
{
	Super::OnUnPossess();

	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ATrooperAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem) return;

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		MIRALOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
}
