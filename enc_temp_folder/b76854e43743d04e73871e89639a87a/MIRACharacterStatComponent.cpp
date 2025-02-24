// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRACharacterStatComponent.h"
#include "MIRAGameInstance.h"

// Sets default values for this component's properties
UMIRACharacterStatComponent::UMIRACharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	Level = 1;
}


// Called when the game starts
void UMIRACharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMIRACharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);

}

void UMIRACharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MIRACHECK(nullptr != MIRAGameInstance);

	CurrentStatData = MIRAGameInstance->GetMIRACharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		//CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		MIRALOG(Error, TEXT("Level (%d) data doesn't Exist"), NewLevel);
	}
}

void UMIRACharacterStatComponent::SetDamage(float NewDamage)
{
	//�������� �ִ� ���̿� ���� ���� �����ִ� �Լ� 
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));

	//if (CurrentHP <= 0.0f)
	//{
	//	OnHPIsZero.Broadcast();
	//}
}

void UMIRACharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnZeroHP.Broadcast();
	}
}

float UMIRACharacterStatComponent::GetPower()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Power;
}

float UMIRACharacterStatComponent::GetHPRatio()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return(CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
