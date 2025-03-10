// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAEnemyStatComponent.h"
#include "MIRAGameInstance.h"

// Sets default values for this component's properties
UMIRAEnemyStatComponent::UMIRAEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

// Called when the game starts
void UMIRAEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMIRAEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UMIRAEnemyStatComponent::SetNewStat(int32 Index)
{
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MIRACHECK(nullptr != MIRAGameInstance);
	CurrentEnemyData = MIRAGameInstance->GetMIRAEnemyCharacterData(Index);
	if (nullptr != CurrentEnemyData)
	{
		SetHP(CurrentEnemyData->MaxHP);
		SetPower(CurrentEnemyData->Power);
	}
	else
	{
		MIRALOG(Error, TEXT("data doesn't Exist"));
	}
}

void UMIRAEnemyStatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentEnemyData->MaxHP));
}

void UMIRAEnemyStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnEnemyHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnEnemyHPZero.Broadcast();
	}
}

void UMIRAEnemyStatComponent::SetPower(float NewPower)
{
	CurrentPower = NewPower;
}

float UMIRAEnemyStatComponent::GetPower()
{
	MIRACHECK(nullptr != CurrentEnemyData, 0.0f);
	return CurrentEnemyData->Power;
}

FString UMIRAEnemyStatComponent::GetEnemyName()
{
	if (nullptr == CurrentEnemyData)
	{
		MIRALOG(Error, TEXT("CurrentEnemyData is nullptr"));
		return FString(TEXT("INVALID"));
	}
	return CurrentEnemyData->EnemyName;
}

FString UMIRAEnemyStatComponent::GetEnemyAlias()
{
	if (nullptr == CurrentEnemyData)
	{
		MIRALOG(Error, TEXT("CurrentEnemyData is nullptr"));
		return FString(TEXT("INVALID"));
	}
	return CurrentEnemyData->Alias;
}

float UMIRAEnemyStatComponent::GetHPRatio()
{
	MIRACHECK(nullptr != CurrentEnemyData, 0.0f);
	return(CurrentEnemyData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentEnemyData->MaxHP);
}
