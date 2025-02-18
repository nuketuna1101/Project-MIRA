// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "MIRAEnemyBaseCharacter.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MIRABlade.generated.h"

UCLASS()
class MIRA_API AMIRABlade : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMIRABlade();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* Weapon;

	UFUNCTION()
	void ApplyBladeAttack();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollider;

	UPROPERTY()
	TArray<AMIRAEnemyBaseCharacter*> OverlappedEnemies;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlapeedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//UFUNCTION()
	//void OnEndOverlap(UPrimitiveComponent* OverlapeedComponent,
	//	AActor* OtherActor,
	//	UPrimitiveComponent* OtherComponent,
	//	int32 OtherBodyIndex);


};