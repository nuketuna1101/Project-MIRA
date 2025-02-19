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

void UMIRABaseCharacterComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));

}

void UMIRABaseCharacterComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
	}
}

float UMIRABaseCharacterComponent::GetPower()
{
	return 0.0f;
}

float UMIRABaseCharacterComponent::GetHPRatio()
{
	return(CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}
