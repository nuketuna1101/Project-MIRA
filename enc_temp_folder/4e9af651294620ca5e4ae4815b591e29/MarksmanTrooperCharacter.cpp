// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanTrooperCharacter.h"
#include "TrooperAnimInstance.h"
#include "Projectile.h"
#include "TrooperAIController.h"
#include "MIRACharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
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

void AMarksmanTrooperCharacter::Attack()
{
	if (!IsAttacking)
	{
		MIRALOG(Warning, TEXT("[AMIRAEnemyBaseCharacter] Attack"));
		// handling by attack montage in anim instance
		TrooperAnim->PlayAttackMontage();
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

float AMarksmanTrooperCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	MIRALOG(Warning, TEXT("Actor: %s / damage: %f"), *GetName(), FinalDamage);

	// TMP: 일단은 조금이라도 피격 당하면 사망으로
	if (FinalDamage > 0.0f)
	{

	}


	return FinalDamage;
}
