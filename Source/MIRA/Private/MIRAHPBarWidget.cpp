// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAHPBarWidget.h"
#include "MIRAEnemyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void UMIRAHPBarWidget::BindCharacterStat(UMIRAEnemyStatComponent* NewEnemyStat)
{
	MIRACHECK(nullptr != NewEnemyStat);

	// Bind HP
	CurrentEnemyStat = NewEnemyStat;
	CurrentDisplayedHP = CurrentEnemyStat->GetHPRatio();
	// bind events-delegates for HP update
	NewEnemyStat->OnEnemyHPChanged.AddUObject(this, &UMIRAHPBarWidget::UpdateHPWidget);
	NewEnemyStat->OnEnemyHPZero.AddUObject(this, &UMIRAHPBarWidget::UpdateHPVisible);

	// set text for actor name
	TextActorName->SetText(FText::FromString(NewEnemyStat->GetEnemyName() + " " + NewEnemyStat->GetEnemyAlias()));
}

void UMIRAHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PBHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	MIRACHECK(nullptr != PBHPBar);

	UpdateHPWidget();
}

void UMIRAHPBarWidget::UpdateHPWidget()
{
	// timer for HP update interpolation
	if (CurrentEnemyStat.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HPUpdateTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			HPUpdateTimerHandle,
			this,
			&UMIRAHPBarWidget::InterpolatedHPUpdate,
			UpdateInterval,
			true
		);
	}
}

void UMIRAHPBarWidget::UpdateHPVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UMIRAHPBarWidget::InterpolatedHPUpdate()
{
	if (!CurrentEnemyStat.IsValid() || !PBHPBar) return;

	// HP amount
	float TargetHP = CurrentEnemyStat->GetHPRatio();
	CurrentDisplayedHP = FMath::FInterpTo(CurrentDisplayedHP, TargetHP, UpdateInterval, InterpSpeed);
	PBHPBar->SetPercent(CurrentDisplayedHP);
	// HP display color
	FLinearColor HPColor = FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Green, CurrentDisplayedHP);
	PBHPBar->SetFillColorAndOpacity(HPColor);

	if (FMath::IsNearlyEqual(CurrentDisplayedHP, TargetHP, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(HPUpdateTimerHandle);
	}
}
