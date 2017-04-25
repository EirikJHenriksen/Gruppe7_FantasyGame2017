// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "BossSpellFire.h"


// Sets default values
ABossSpellFire::ABossSpellFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision object and RootObject
	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MySphere"));
	RootComponent = RootSphere;
	RootSphere->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
}

// Called when the game starts or when spawned
void ABossSpellFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossSpellFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

