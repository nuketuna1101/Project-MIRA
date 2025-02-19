// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MIRAGameInstance.generated.h"

// imported from csv data table
USTRUCT()
struct FMIRACharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FMIRACharacterData() : Level(1), MaxHP(200.0f), MaxMP(80.0f), Power(10.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Data")
	float Power;
};

/**
 *
 */

// game instance
UCLASS()
class MIRA_API UMIRAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMIRAGameInstance();

	virtual void Init() override;
	FMIRACharacterData* GetMIRACharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* MIRACharacterDataTable;
	
	void PrintMIRACharacterDataAll();
};
