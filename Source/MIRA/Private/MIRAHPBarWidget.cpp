// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAHPBarWidget.h"
#include "MIRACharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void UMIRAHPBarWidget::BindCharacterStat(UMIRACharacterStatComponent* NewCharaterStat)
{
	MIRACHECK(nullptr != NewCharaterStat);

	// Bind HP
	CurrentCharacterStat = NewCharaterStat;
	CurrentDisplayedHP = CurrentCharacterStat->GetHPRatio();
	// bind events-delegates for HP update
	NewCharaterStat->OnHPChanged.AddUObject(this, &UMIRAHPBarWidget::UpdateHPWidget);
	NewCharaterStat->OnZeroHP.AddUObject(this, &UMIRAHPBarWidget::UpdateHPVisible);

	// set text for actor name
	//TextActorName->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLevel())));
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

	// HP amount
	float TargetHP = CurrentCharacterStat->GetHPRatio();
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
