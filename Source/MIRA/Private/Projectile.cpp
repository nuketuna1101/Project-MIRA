// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	_LifeCountingDown = LifeSpan;

	MIRALOG(Warning, TEXT("projectile beginplayed"));
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_LifeCountingDown > 0.0f)
	{
		// update transform
		//FVector currentLocation = GetActorLocation();
		//FVector vel = GetActorRotation().RotateVector(FVector::ForwardVector) * Speed * DeltaTime;
		//FVector nextLocation = currentLocation + vel;
		//SetActorLocation(nextLocation);

		////Ray cast Check 
		//FHitResult hitResult;
		//FCollisionObjectQueryParams objCollisionQueryParams;
		//objCollisionQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		//// check if first collided
		//if (GetWorld()->LineTraceSingleByObjectType(hitResult, currentLocation, nextLocation, objCollisionQueryParams))
		//{
		//	auto TheActor = hitResult.GetActor();
		//	if (hitResult.GetActor() != nullptr)
		//	{
		//		MIRALOG(Warning, TEXT("[Projectile] hit called"));

		//		// remove it
		//		PrimaryActorTick.bCanEverTick = false;
		//		Destroy();
		//	}
		//}

		////Reduce time
		//_LifeCountingDown -= DeltaTime;
	}
	else
	{
		// if out of time, remove it
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}
}