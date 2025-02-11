// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRAEnemyBaseCharacter.h"
#include "MarksmanTrooperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMarksmanTrooperCharacter : public AMIRAEnemyBaseCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMarksmanTrooperCharacter();

	// attack
	virtual void Attack() override;

	// damage framework
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UClass* BulletClass;

};
