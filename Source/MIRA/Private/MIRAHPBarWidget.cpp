// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAHPBarWidget.h"
#include "MIRACharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void UMIRAHPBarWidget::BindCharacterStat(UMIRACharacterStatComponent* NewCharaterStat)
{
	MIRACHECK(nullptr != NewCharaterStat);

	CurrentCharacterStat = NewCharaterStat;
	CurrentDisplayedHP = CurrentCharacterStat->GetHPRatio();

	NewCharaterStat->OnHPChanged.AddUObject(this, &UMIRAHPBarWidget::UpdateHPWidget);
	NewCharaterStat->OnZeroHP.AddUObject(this, &UMIRAHPBarWidget::UpdateHPVisible);


	TextActorName->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLevel())));
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
	if (CurrentCharacterStat.IsValid())
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
	if (!CurrentCharacterStat.IsValid() || !PBHPBar) return;

	float TargetHP = CurrentCharacterStat->GetHPRatio();
	CurrentDisplayedHP = FMath::FInterpTo(CurrentDisplayedHP, TargetHP, UpdateInterval, InterpSpeed);

	PBHPBar->SetPercent(CurrentDisplayedHP);

	FLinearColor HPColor = FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Green, CurrentDisplayedHP);
	PBHPBar->SetFillColorAndOpacity(HPColor);

	if (FMath::IsNearlyEqual(CurrentDisplayedHP, TargetHP, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(HPUpdateTimerHandle);
	}
}
