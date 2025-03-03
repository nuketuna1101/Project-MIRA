// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacter.h"
#include "MIRABossIvan.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRABossIvan : public AMIRABaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMIRABossIvan();
	
	//virtual void PostInitializeComponents() override;


protected:
	virtual void BeginPlay() override;

	class AMIRAPlayerCharacter* Target = nullptr;

	UClass* BulletClass;
};
