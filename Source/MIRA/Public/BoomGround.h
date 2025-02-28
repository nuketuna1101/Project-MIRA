// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "BoomGround.generated.h"

UCLASS()
class MIRA_API ABoomGround : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoomGround();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle BoomTimerHandle;

	float ElapsedTime;
	float UpdateTime;
	float BoomTime;

	void BoomTimerUpdate();

	void BoomExplode();

public:	

#pragma region SubComponents
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;
#pragma endregion

	// dynamic mateiral instance for lerping color
	UMaterialInstanceDynamic* DynamicMaterial; 
};
