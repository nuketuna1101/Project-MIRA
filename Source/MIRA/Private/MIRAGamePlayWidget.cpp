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

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnReturenToTitleClicked);
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

void UMIRAGamePlayWidget::OnReturenToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}
