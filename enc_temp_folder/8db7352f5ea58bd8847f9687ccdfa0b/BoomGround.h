// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "BoomGround.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBGOnExplodeDelegate);


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

#pragma region TimerHandle Variables

	FTimerHandle BoomTimerHandle;
	float ElapsedTime;
	float UpdateTime;
	float BoomTime;

#pragma endregion

#pragma region Damage Frameworks varaibles

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageRadius = 500.0f;

#pragma endregion

	bool bIsBoomed;

	void BoomTimerUpdate();

	void BoomExplode();

public:	

	FBGOnExplodeDelegate BGOnExplodeBP;

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
