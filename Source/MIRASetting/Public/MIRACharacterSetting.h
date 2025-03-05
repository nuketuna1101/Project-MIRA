// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MIRACharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=MIRA)
class MIRASETTING_API UMIRACharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UMIRACharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterMeshAssets;
	
	UPROPERTY(config)
	TArray<FSoftObjectPath> TrooperMeshAssets;

	UPROPERTY(config)
	TArray<FSoftObjectPath> BossMeshAssets;

	UPROPERTY(config)
	TArray<FSoftObjectPath> BossEFXAssets;

	UPROPERTY(config)
	TArray<FSoftObjectPath> BossSFXAssets;
};
