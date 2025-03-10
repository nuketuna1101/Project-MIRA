// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRABossHUDWidget.h"
#include "MIRABossIvan.h"
#include "MIRAEnemyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMIRABossHUDWidget::BindBossStat(UMIRAEnemyStatComponent* BossStat)
{
	MIRACHECK(nullptr != BossStat);
	CurrentBossStat = BossStat;
	BossStat->OnEnemyHPChanged.AddUObject(this, &UMIRABossHUDWidget::UpdateBossStat);
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

void UMIRABossHUDWidget::UpdateBossStat()
{
	MIRACHECK(CurrentBossStat.IsValid());
	HPBar->SetPercent(CurrentBossStat->GetHPRatio());
}
