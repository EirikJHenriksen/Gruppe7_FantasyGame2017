// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "CollectionPickup.h"


// Sets default values
ACollectionPickup::ACollectionPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MySphere"));
	RootComponent = RootSphere;
}

// Called when the game starts or when spawned
void ACollectionPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectionPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	++Rotator;
	SetActorRotation(FRotator(0.f, (1.f * Rotator), 0.f));

	if (Rotator >= 360)
	{
		Rotator = 0;
	}
}

