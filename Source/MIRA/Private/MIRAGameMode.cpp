// Copyright Epic Games, Inc. All Rights Reserved.

#include "MIRAGameMode.h"
#include "MIRAPlayerCharacter.h"
#include "MIRAPawn.h"
#include "MIRAPlayerController.h"
#include "MIRAPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AMIRAGameMode::AMIRAGameMode()
{
	// settings for default class
	DefaultPawnClass = AMIRAPlayerCharacter::StaticClass();
	PlayerControllerClass = AMIRAPlayerController::StaticClass();
	PlayerStateClass = AMIRAPlayerState::StaticClass();

	ProjectilePool = CreateDefaultSubobject<UPoolComponent>(TEXT("ProjectilePool"));
	static ConstructorHelpers::FClassFinder<AActor> 
		ProjectileBPClassFinder(TEXT("/Game/MIRA/Characters/Blueprints/BP_MIRAPlayerProjectile.BP_MIRAPlayerProjectile_C"));
	if (ProjectileBPClassFinder.Class != nullptr)
	{
		ProjectilePool->ObjectClass = ProjectileBPClassFinder.Class;
	}
}

void AMIRAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// post login on game mode
	Super::PostLogin(NewPlayer);

	auto MIRAPlayerState = Cast<AMIRAPlayerState>(NewPlayer->PlayerState);
	MIRACHECK(nullptr != MIRAPlayerState);
	MIRAPlayerState->InitPlayerData();

	if (ProjectilePool.IsValid())
	{
		MIRALOG(Warning, TEXT("PostLogin ProjectilePool 컴포넌트 생성 성공"));
	}
	else
	{
		MIRALOG(Error, TEXT("PostLogin ProjectilePool 컴포넌트 생성 실패"));
	}
}
