// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "TrooperAIController.h"
#include "MIRACharacter.h"
#include "MIRAEnemyBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("DetectPlayer");
	Interval = 1.0f;

	// for detections, there are some constraints
	// sight range and sight FOV
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Procedure of Detection
	// 1. sphere trace : check if player exists in trooper nearby
	// 2. line trace : trooper-player check => is valid in FOV? and is NOT blocked by anything

	// npc ai pawn
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World) return;

	FVector TrooperLocation = ControllingPawn->GetActorLocation();

	float DetectRadius = 800.0f;

	// step1. sphere trace
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape DetectSphere = FCollisionShape::MakeSphere(DetectRadius);
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bSphereTraceResult = World->OverlapMultiByChannel(
		OverlapResults,
		TrooperLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		DetectSphere,
		CollisionQueryParam
	);
	DrawDebugSphere(World, TrooperLocation, DetectRadius, 16, FColor::Green, false, 0.2f);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	// if nothing on sphere raycast, missed
	if (!bSphereTraceResult)
	{
		MIRALOG(Warning, TEXT("[Detect] not detect on sphere: set value as null"));
		BBComp->SetValueAsObject(ATrooperAIController::TargetKey, nullptr);
	}
	else
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			// check the overlapped thing is Player	
			AActor* Overlapped = OverlapResult.GetActor();
			if (!Overlapped || Overlapped == ControllingPawn) continue;
			AMIRACharacter* PlayerCharacter = Cast<AMIRACharacter>(Overlapped);
			if (!PlayerCharacter) continue;

			FVector PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector PlayerDir = (PlayerLocation - TrooperLocation).GetSafeNormal();
			FVector TrooperDir = ControllingPawn->GetActorForwardVector();
			float dotProductDirs = FVector::DotProduct(PlayerDir, TrooperDir);
			float CosFOV = FMath::Cos(FMath::DegreesToRadians(60));
			// out of FOV, ignore
			if (dotProductDirs < CosFOV)	continue;

			// Debug draw FOV cone
			FVector FOVLineEnd = TrooperLocation + TrooperDir * DetectRadius;
			DrawDebugLine(World, TrooperLocation, FOVLineEnd, FColor::Red, false, DeltaSeconds, 0, 2.0f);
			FVector FOVLeft = TrooperLocation + TrooperDir.RotateAngleAxis(-60, FVector(0, 0, 1)) * DetectRadius;
			FVector FOVRight = TrooperLocation + TrooperDir.RotateAngleAxis(60, FVector(0, 0, 1)) * DetectRadius;
			DrawDebugLine(World, TrooperLocation, FOVLeft, FColor::Yellow, false, DeltaSeconds, 0, 1.0f);
			DrawDebugLine(World, TrooperLocation, FOVRight, FColor::Yellow, false, DeltaSeconds, 0, 1.0f);



			// step2. Line trace
			FHitResult HitResults;
			CollisionQueryParam.AddIgnoredActor(Overlapped);

			bool bLineTraceResult = World->LineTraceSingleByChannel(
				HitResults,
				TrooperLocation,
				PlayerLocation,
				ECollisionChannel::ECC_EngineTraceChannel2,
				CollisionQueryParam
			);

			// if not blocked, or 
			if (!bLineTraceResult)
			{
				DrawDebugLine(World, TrooperLocation, PlayerLocation, FColor::Blue, false, 0.1f, 0, 2.0f);
				BBComp->SetValueAsObject(ATrooperAIController::TargetKey, PlayerCharacter);
				MIRALOG(Warning, TEXT("[Detect] success"));
				return;
			}
		}
		MIRALOG(Warning, TEXT("[Detect] set value as null"));
		BBComp->SetValueAsObject(ATrooperAIController::TargetKey, nullptr);
	}
}