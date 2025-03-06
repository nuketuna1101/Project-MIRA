// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckPlayerDistance.h"
#include "MIRAAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTService_CheckPlayerDistance::UBTService_CheckPlayerDistance()
{
	NodeName = TEXT("CheckPlayerDistance");
}

void UBTService_CheckPlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr)		return;
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	BBComp->SetValueAsObject(AMIRAAIController::TargetActorKey, PlayerPawn);

	FVector DistanceVector = ControllingPawn->GetActorLocation() - PlayerPawn->GetActorLocation();
	float Distance = DistanceVector.Size();
	BBComp->SetValueAsFloat(AMIRAAIController::TargetDistanceKey, Distance);
}
