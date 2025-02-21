// Fill out your copyright notice in the Description page of Project Settings.


#include "MIRAPlayerController.h"
#include "MIRAHUDWidget.h"
#include "MIRAGamePlayWidget.h"

AMIRAPlayerController::AMIRAPlayerController()
{
	// widget binding
	static ConstructorHelpers::FClassFinder<UMIRAHUDWidget>
		UI_HUD(TEXT("/Game/MIRA/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD.Succeeded())
	{
		HUDWidgetClass = UI_HUD.Class;
	}
	else
	{
		MIRALOG(Error, TEXT("Failed to find HUD widget class!"));
	}

	static ConstructorHelpers::FClassFinder<UMIRAGamePlayWidget>
		UI_GAMEPLAY(TEXT("/Game/MIRA/UI/UI_GamePlay.UI_GamePlay_C"));
	if (UI_GAMEPLAY.Succeeded())
	{
		GamePlayWidgetClass = UI_GAMEPLAY.Class;
	}
	else
	{
		MIRALOG(Error, TEXT("Failed to find UI_GAMEPLAY class!"));
	}
}

void AMIRAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// exclude ui and only send input to game
	/*FInputModeGameOnly InputMode;
	SetInputMode(InputMode);*/
	ChangeInputMode(true);


	//
	HUDWidget = CreateWidget<UMIRAHUDWidget>(this, HUDWidgetClass);
	MIRACHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);
}

void AMIRAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMIRAPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMIRAPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(InputModeGame);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(InputModeUI);
		bShowMouseCursor = true;
	}
}

void AMIRAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// bind action mapping
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed,
		this, &ThisClass::OnGamePause);
}

void AMIRAPlayerController::OnGamePause()
{
	GamePlayWidget = CreateWidget<UMIRAGamePlayWidget>(this, GamePlayWidgetClass);
	MIRACHECK(nullptr != GamePlayWidget);
	GamePlayWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

