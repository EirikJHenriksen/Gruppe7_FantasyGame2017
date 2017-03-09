// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "ConeOfFire.h"


// Sets default values
AConeOfFire::AConeOfFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision object and RootObject
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBox"));
	RootComponent = RootBox;
	RootBox->bGenerateOverlapEvents = true;

	//Change scale of box.
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

	// Lag en release funksjon som gjør at flammen eksisterer så lenge knappen holdes inne.

	TimeLived += DeltaTime;
	if (TimeLived > TimeBeforeDestroy)
	{
		this->Destroy();
	}
}

