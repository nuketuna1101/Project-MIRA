// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanTrooperCharacter.h"
#include "TrooperAnimInstance.h"
#include "Projectile.h"
#include "TrooperAIController.h"
#include "MIRACharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMarksmanTrooperCharacter::AMarksmanTrooperCharacter()
{
	// transform setting
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_MARKSMANTROOPER(TEXT("/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith"));
	if (SK_MARKSMANTROOPER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MARKSMANTROOPER.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MARKSMANTROOPER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooperAnimBlueprint.MarksmanTrooperAnimBlueprint_C"));
	if (MARKSMANTROOPER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MARKSMANTROOPER_ANIM.Class);
	}



	// temp bullet
	static ConstructorHelpers::FObjectFinder<UBlueprint>
		blueprint_finder(TEXT("Blueprint'/Game/MIRA/Characters/Blueprints/BP_TrooperBullet.BP_TrooperBullet'"));
	BulletClass = (UClass*)blueprint_finder.Object->GeneratedClass;

}

void AMarksmanTrooperCharacter::Attack()
{
	if (!IsAttacking)
	{
		MIRALOG(Warning, TEXT("[AMIRAEnemyBaseCharacter] Attack"));
		// handling by attack montage in anim instance
		TrooperAnim->PlayAttackMontage();
		IsAttacking = true;

		// bullet
		//auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(AProjectile::StaticClass()));
		auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
		auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
		//auto Target = Cast<AMIRACharacter>(BBComp->GetValueAsObject(ATrooperAIController::TargetKey));
		if (nullptr == TargetPlayer)	return;

		FVector TrooperLocation = GetActorLocation();
		TrooperLocation.Z += 50.0f;
		FVector targetLocation = TargetPlayer->GetActorLocation();
		targetLocation.Z = TrooperLocation.Z;
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(TrooperLocation, targetLocation);
		// 로그 출력
		MIRALOG(Warning, TEXT("TrooperLocation: %s"), *TrooperLocation.ToString());
		MIRALOG(Warning, TEXT("targetLocation: %s"), *targetLocation.ToString());

		Bullet->SetActorLocation(TrooperLocation);
		Bullet->SetActorRotation(rotation);
	}
	else
	{
		MIRALOG(Warning, TEXT("Already attacking, skipping new attack."));
	}

}
