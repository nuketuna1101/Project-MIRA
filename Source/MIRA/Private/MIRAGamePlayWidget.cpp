// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAGamePlayWidget.h"
#include "Components/Button.h"
#include "MIRAPlayerController.h"

void UMIRAGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// click event binding
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ThisClass::OnResumeClicked);
	}

	ControlInfoButton = Cast<UButton>(GetWidgetFromName(TEXT("btnControlInfo")));
	if (nullptr != ControlInfoButton)
	{
		ControlInfoButton->OnClicked.AddDynamic(this, &ThisClass::OnControlInfoClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnReturenToTitleClicked);
	}

	// control info widget
	ControlInfoWidget = CreateWidget<UUserWidget>(
		GetWorld(), 
		LoadClass<UUserWidget>(GetWorld(), TEXT("/Game/MIRA/UI/UI_ControlInfo.UI_ControlInfo_C")));
	if (ControlInfoWidget)
	{
		// z-order important
		ControlInfoWidget->AddToViewport(10);
		ControlInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMIRAGamePlayWidget::OnResumeClicked()
{
	auto MIRAPlayerController = Cast<AMIRAPlayerController>(GetOwningPlayer());
	MIRACHECK(nullptr != MIRAPlayerController);

	RemoveFromParent();
	MIRAPlayerController->SetInputMode(FInputModeGameOnly());
	MIRAPlayerController->SetPause(false);
}

void UMIRAGamePlayWidget::OnControlInfoClicked()
{
	// to do : toggle control info widget
	if (ControlInfoWidget)
	{
		if (ControlInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			ControlInfoWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ControlInfoWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMIRAGamePlayWidget::OnReturenToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
