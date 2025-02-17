// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRACharacterStatComponent.h"

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
}
