// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMIRABossHUDWidget::BindBossStat(UMIRACharacterStatComponent* CharacterStat)
{
}

void UMIRABossHUDWidget::BindBossState(AMIRAPlayerState* PlayerState)
{
}

void UMIRABossHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbBossHP")));
	MIRACHECK(nullptr != HPBar);

	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtBossName")));
	MIRACHECK(nullptr != BossName);

	BossAlias = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtBossAlias")));
	MIRACHECK(nullptr != BossAlias);
}
