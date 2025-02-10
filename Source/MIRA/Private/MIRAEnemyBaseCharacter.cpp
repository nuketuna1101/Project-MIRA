// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAEnemyBaseCharacter.h"
#include "Components/WidgetComponent.h"
#include "TrooperAIController.h"
#include "TrooperAnimInstance.h"

// Sets default values
AMIRAEnemyBaseCharacter::AMIRAEnemyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create default subobject
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));

	// hierarchy
	HPBar->SetupAttachment(GetMesh());

	// transform
	HPBar->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);

	// set widget
	static ConstructorHelpers::FClassFinder<UUserWidget> 
		UI_HPBAR(TEXT("/Game/MIRA/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HPBAR.Succeeded())
	{
		HPBar->SetWidgetClass(UI_HPBAR.Class);
		HPBar->SetDrawSize(FVector2D(300.0f, 100.0f));
	}

	// ai controller
	AIControllerClass = ATrooperAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMIRAEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMIRAEnemyBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TrooperAnim = Cast<UTrooperAnimInstance>(GetMesh()->GetAnimInstance());
	MIRACHECK(nullptr != TrooperAnim);

	// event binding on montage end
	if (!TrooperAnim) return;
	TrooperAnim->OnMontageEnded.AddDynamic(this, &AMIRAEnemyBaseCharacter::OnAttackMontageEnded);
}

void AMIRAEnemyBaseCharacter::Attack()
{
	if (!IsAttacking)
	{
		MIRALOG(Warning, TEXT("[AMIRAEnemyBaseCharacter] Attack"));
		// handling by attack montage in anim instance
		TrooperAnim->PlayAttackMontage();
		IsAttacking = true;
	}
	else
	{
		MIRALOG(Warning, TEXT("Already attacking, skipping new attack."));
	}
}

void AMIRAEnemyBaseCharacter::SetTarget(AMIRACharacter* TargetActor)
{
	Target = TargetActor;
}

void AMIRAEnemyBaseCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	MIRACHECK(IsAttacking);
	IsAttacking = false;
	// delegate broadcast
	OnAttackEnd.Broadcast();
}
