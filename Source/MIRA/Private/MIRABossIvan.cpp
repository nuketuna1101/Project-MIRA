// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossIvan.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAAIController.h"
#include "MIRACharacterSetting.h"

AMIRABossIvan::AMIRABossIvan()
{
	// [DONT HAVE TO] Skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_IVAN(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant"));
	if (SK_IVAN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_IVAN.Object);
	}

	// setting for animations
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		IVAN_ANIM(TEXT("/Game/MIRA/Characters/Animations/Ivan/ABP_Ivan.ABP_Ivan_C"));
	if (IVAN_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(IVAN_ANIM.Class);
	}

	// ai controller
	//AIControllerClass = ATrooperAIController::StaticClass();
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	ConstructorHelpers::FClassFinder<AMIRAAIController>
		BPCLASS_IVANAICONTROLLER(TEXT("/Game/MIRA/Characters/AI/BP_IvanAIController.BP_IvanAIController_C"));
	if (BPCLASS_IVANAICONTROLLER.Succeeded())
	{
		AIControllerClass = BPCLASS_IVANAICONTROLLER.Class;
	}
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// collision setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
}

void AMIRABossIvan::BeginPlay()
{
	Super::BeginPlay();

	// Asset loading
	auto DefaultSetting = GetDefault<UMIRACharacterSetting>();
	CharacterAssetToLoad = DefaultSetting->BossAssets[0];
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(GetGameInstance());
	MIRACHECK(nullptr != MIRAGameInstance);
	AssetStreamingHandle = MIRAGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AMIRABaseCharacter::
			OnAssetLoadCompleted));

	// set player as Target
	auto TargetPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	Target = Cast<AMIRAPlayerCharacter>(TargetPawn);
}
