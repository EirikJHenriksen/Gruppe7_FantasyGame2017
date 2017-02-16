// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "ManaPotion.h"


// Sets default values
AManaPotion::AManaPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManaPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AManaPotion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

