// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRABaseCharacter.h"
#include "MIRABossIvan.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossAliveOrDeath, bool);

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
	
	virtual void PostInitializeComponents() override;

#pragma region CharacterState

	FOnBossAliveOrDeath OnBossAliveOrDeath;

	void SetCharacterState(ECharacterState NewState) override;
#pragma endregion

#pragma region Skill Pattern

	void FireHomings();

	void ThrowBoomGrounds();

	void LaunchBoomGrounds();

	void CastDeathStare();

	void DeathStare();

	void StartVanish();

	void EndVanish();
#pragma endregion


protected:
	virtual void BeginPlay() override;

	class AMIRAPlayerCharacter* Target = nullptr;

	UClass* BulletClass;
	UClass* BGClass;

#pragma region Anim Instance
	UPROPERTY()
	class UIvanAnimInstance* IvanAnim;
#pragma endregion

#pragma region EFX and SFX
	UParticleSystem* EFX_MuzzleFire;
	UParticleSystem* EFX_ThrowTrail;
	UParticleSystem* EFX_Casting;
	UParticleSystem* EFX_DeathStare;
	UParticleSystem* EFX_Vanish;

	USoundBase* SFX_IvanFire;
	USoundBase* SFX_Throw;
	USoundBase* SFX_Casting;
	USoundBase* SFX_DeathStare;
	USoundBase* SFX_StartVanish;
	USoundBase* SFX_EndVanish;

#pragma endregion
};
