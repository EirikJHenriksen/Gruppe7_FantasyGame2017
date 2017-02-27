// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "PowerUp_Speed.h"


// Sets default values
APowerUp_Speed::APowerUp_Speed()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision object and RootObject
	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MySphere"));
	RootComponent = RootSphere;
}

// Called when the game starts or when spawned
void APowerUp_Speed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp_Speed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

