// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MIRA.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MIRAProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPOnHitDelegate, FVector, HitLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPOnDeadDelegate, FVector, DeadLocation);

UCLASS()
class MIRA_API AMIRAProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMIRAProjectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    float CurLifeCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    bool bIsMIRAHoming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    AActor* HomingTarget;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Projectile")
    float LifeSpan = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<AActor> TargetClass;

#pragma region [TO DO] Delegates for BP event
    // delegate for when be hitted
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FPOnHitDelegate POnHitBP;

    UPROPERTY(BlueprintAssignable, Category = "Event")
    FPOnDeadDelegate POnDeadBP;
    
#pragma endregion


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

};
