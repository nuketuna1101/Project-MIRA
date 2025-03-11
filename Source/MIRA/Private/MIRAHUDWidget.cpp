// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MIRACharacterStatComponent.h"
#include "MIRAPlayerState.h"

void UMIRAHUDWidget::BindCharacterStat(UMIRACharacterStatComponent* CharacterStat)
{
	MIRACHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UMIRAHUDWidget::UpdateCharacterHP);
	CharacterStat->OnBulletChanged.AddUObject(this, &UMIRAHUDWidget::UpdatePlayerBullets);
}

void UMIRAHUDWidget::BindPlayerState(AMIRAPlayerState* PlayerState)
{
	MIRACHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateUpdate.AddUObject(this, &UMIRAHUDWidget::UpdatePlayerState);
}

void UMIRAHUDWidget::ShowBossHUD(bool bIsShowed)
{
	if (BossHUDWidget)
	{
		BossHUDWidget->SetVisibility(bIsShowed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
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

	BossHUDWidget = Cast<UMIRABossHUDWidget>(GetWidgetFromName(TEXT("UI_MIRABossHUD")));
	MIRACHECK(nullptr != BossHUDWidget);
	if (BossHUDWidget)
	{
		BossHUDWidget->SetVisibility(ESlateVisibility::Hidden);
		//BossHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}

	BulletImages.Empty();
	for (int32 i = 0; i < 3; ++i)
	{
		FString WidgetName = FString::Printf(TEXT("imgBullet%d"), i);
		auto BulletImage = Cast<UImage>(GetWidgetFromName(FName(*WidgetName)));
		if (BulletImage)
		{
			BulletImages.Add(BulletImage);
		}
		else
		{
			MIRALOG(Warning, TEXT("BulletImage %s not found!"), *WidgetName);
		}
	}
	MIRACHECK(BulletImages.Num() > 0);
}

void UMIRAHUDWidget::UpdateCharacterHP()
{
	// stat data updates
	MIRACHECK(CurrentCharacterStat.IsValid());
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UMIRAHUDWidget::UpdatePlayerState()
{
	MIRACHECK(CurrentPlayerState.IsValid());

	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(
		FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
}

void UMIRAHUDWidget::UpdatePlayerBullets()
{
	MIRACHECK(CurrentCharacterStat.IsValid());
	int32 AvailableBulletConstraint = CurrentCharacterStat->GetCurrentBullets() - 1;
	for (int i = 0; i < CurrentCharacterStat->GetMaxBullets(); ++i)
	{
		BulletImages[i]->SetColorAndOpacity((
			(i <= AvailableBulletConstraint) ?
			AvailableBulletColor :
			EmptyBulletColor
			));
	}
}
