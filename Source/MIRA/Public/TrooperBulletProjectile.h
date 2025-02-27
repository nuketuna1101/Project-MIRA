// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TrooperBulletProjectile.generated.h"

UCLASS()
class MIRA_API ATrooperBulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrooperBulletProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    float CurLifeCount;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

#pragma region SubComponents
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UParticleSystemComponent* ParticleComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UArrowComponent* ArrowComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;
#pragma endregion

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float LifeSpan = 3.0f;
};
