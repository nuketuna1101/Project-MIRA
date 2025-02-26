// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAMarksmanTrooper.h"
#include "TrooperAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

AMIRAMarksmanTrooper::AMIRAMarksmanTrooper()
{
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
		MARKSMANTROOPER_ANIM(TEXT("/Game/MIRA/Characters/Animations/MarksmanTrooper/MarksmanTrooperAnimBlueprint.MarksmanTrooperAnimBlueprint_C"));
	if (MARKSMANTROOPER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MARKSMANTROOPER_ANIM.Class);
	}

	// temp bullet
	static ConstructorHelpers::FObjectFinder<UBlueprint>
		blueprint_finder(TEXT("Blueprint'/Game/MIRA/Characters/Blueprints/BP_TrooperBullet.BP_TrooperBullet'"));
	BulletClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

void AMIRAMarksmanTrooper::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//MIRAAnim = Cast<UTrooperAnimInstance>(GetMesh()->GetAnimInstance());
	//MIRACHECK(nullptr != MIRAAnim);
	//if (!MIRAAnim) return;
	//MIRAAnim->OnMontageEnded.AddDynamic(this, &AMIRAMarksmanTrooper::OnAttackMontageEnded);
}

void AMIRAMarksmanTrooper::Attack()
{
	if (!IsAttacking)
	{
		// handling by attack montage in anim instance
		//MIRAAnim->PlayAttackMontage();
		IsAttacking = true;

		// bullet
		auto Bullet = Cast<AActor>(GetWorld()->SpawnActor(BulletClass));
		auto TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (nullptr == TargetPlayer)	return;

		FVector TrooperLocation = GetActorLocation();
		TrooperLocation.Z += 50.0f;
		FVector TargetLocation = TargetPlayer->GetActorLocation();
		TargetLocation.Z = TrooperLocation.Z;
		FVector BulletDir = (TargetLocation - TrooperLocation).GetSafeNormal();


		Bullet->SetActorLocation(TrooperLocation);

		UProjectileMovementComponent* ProjectileMovement = Bullet->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			FVector BulletVel = BulletDir * 800.0f;
			ProjectileMovement->SetVelocityInLocalSpace(BulletVel);
		}
	}
	else
	{
		MIRALOG(Warning, TEXT("Already attacking, skipping new attack."));
	}
}

void AMIRAMarksmanTrooper::SetTarget(AMIRACharacter* TargetActor)
{
	Target = TargetActor;
}

void AMIRAMarksmanTrooper::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	//OnAttackEnd.Broadcast();
	OnAttackEndBP.Broadcast();
}

void AMIRAMarksmanTrooper::BeginPlay()
{
	Super::BeginPlay();

	// 임시
	//TrooperStat->SetNewStat("MarksmanTrooper");
}
