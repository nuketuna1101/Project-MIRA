// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMIRAHUDWidget::BindCharacterStat(UABCharacterStatComponent* CharacterStat)
{
}

void UMIRAHUDWidget::BindPlayerState(AABPlayerState* PlayerState)
{
}

void UMIRAHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	MIRACHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	MIRACHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	MIRACHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	MIRACHECK(nullptr != PlayerLevel);
}

void UMIRAHUDWidget::UpdateCharacterStat()
{
}

void UMIRAHUDWidget::UpdatePlayerState()
{
}
