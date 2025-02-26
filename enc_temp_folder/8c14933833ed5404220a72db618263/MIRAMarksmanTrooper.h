// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIRABaseCharacter.h"
#include "MIRAMarksmanTrooper.generated.h"

/**
 * 
 */
UCLASS()
class MIRA_API AMIRAMarksmanTrooper : public AMIRABaseCharacter
{
	GENERATED_BODY()
	
public:
	AMIRAMarksmanTrooper();

	virtual void PostInitializeComponents() override;


	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	//	AController* EventInstigator, AActor* DamageCauser) override;

	// attack
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetTarget(AMIRACharacter* TargetActor);

	// anim montage
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ui widgets
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBar;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

protected:
	virtual void BeginPlay() override;

#pragma region Asset Async Loading by Streamable Manager
	//virtual void OnAssetLoadCompleted() override;
	//FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	//TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
#pragma endregion

	class AMIRACharacter* Target = nullptr;

	UClass* BulletClass;

};
