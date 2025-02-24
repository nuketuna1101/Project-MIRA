// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABaseCharacterComponent.h"
#include "MIRAGameInstance.h"

// Sets default values for this component's properties
UMIRABaseCharacterComponent::UMIRABaseCharacterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}

// Called when the game starts
void UMIRABaseCharacterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMIRABaseCharacterComponent::InitializeComponent()
{
	Super::InitializeComponent();
}



void UMIRABaseCharacterComponent::SetNewStat()
{
	auto MIRAGameInstance = Cast<UMIRAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MIRACHECK(nullptr != MIRAGameInstance);
	CurrentStatData = MIRAGameInstance->GetMIRACharacterData(1);
	if (nullptr != CurrentStatData)
	{
		//Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		//CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		MIRALOG(Error, TEXT("data doesn't Exist"));
	}


	/*
	auto ABGameInstance = Cast<UABGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != ABGameInstance);
	CurrentStatData = ABGameInstance->GetABCharacterData(NewLevel);

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		//CurrentHP = CurrentStatData->MaxHP;
	}
	else
	{
		ABLOG(Error, TEXT("Level (%d) data doesn't Exist"), NewLevel);
	}
	*/
}

void UMIRABaseCharacterComponent::SetDamage(float NewDamage)
{
	MIRACHECK(nullptr != CurrentStatData);

	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UMIRABaseCharacterComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnZeroHP.Broadcast();
	}
}

float UMIRABaseCharacterComponent::GetPower()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Power;
}

float UMIRABaseCharacterComponent::GetHPRatio()
{
	MIRACHECK(nullptr != CurrentStatData, 0.0f);
	return(CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
