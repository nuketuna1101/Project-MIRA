// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAUserWidget.h"
#include "MIRAPlayerController.h"
#include "Components/Button.h"

void UMIRAUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &ThisClass::OnResumeClicked);
	}
}

void UMIRAUserWidget::OnResumeClicked()
{
	auto MIRAPlayerController = Cast<AMIRAPlayerController>(GetOwningPlayer());
	RemoveFromParent();
	MIRAPlayerController->SetInputMode(FInputModeGameOnly());
	MIRAPlayerController->SetPause(false);
}
