// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
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

// imported from csv data table
USTRUCT()
struct FMIRAEnemyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMIRAEnemyCharacterData() : EnemyName("Trooper"), Alias(""), MaxHP(100.0f), Power(10.0f), IsBoss(false) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	FString Alias;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
	bool IsBoss;
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
	FMIRAEnemyCharacterData* GetMIRAEnemyCharacterData(FString EnemyName);

	// StreamableManager for Asset Async loading
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* MIRACharacterDataTable;
	
	UPROPERTY()
	class UDataTable* MIRAEnemyCharacterDataTable;

	void PrintMIRACharacterDataAll();
	void PrintMIRAEnemyCharacterDataAll();
};
