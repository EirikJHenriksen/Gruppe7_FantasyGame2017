// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "ConeOfFire.h"


// Sets default values
AConeOfFire::AConeOfFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation.
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("MProjectile"));

	//CollisionComponent->OnComponentHit.AddDynamic(this, &AMagicProjectile::OnHit);

	//CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnOverlap);

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = 1.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AConeOfFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConeOfFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Makes the fire grow over time. Starts small and ends up big.
	float CurrentScale = CollisionComponent->GetComponentScale().X;
	CurrentScale += DeltaTime;
	CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
	CollisionComponent->SetWorldScale3D(FVector(CurrentScale));

	// Lag en release funksjon som gj�r at flammen eksisterer s� lenge knappen holdes inne.

	TimeLived += DeltaTime;
	if (TimeLived > TimeBeforeDestroy)
	{
		this->Destroy();
	}
}

