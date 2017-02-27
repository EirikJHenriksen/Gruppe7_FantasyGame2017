// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "PhysAttackBox.h"


// Sets default values
APhysAttackBox::APhysAttackBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision object and RootObject
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MyBox"));
	RootComponent = RootBox;
	RootBox->bGenerateOverlapEvents = true;

}

// Called when the game starts or when spawned
void APhysAttackBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysAttackBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLived += DeltaTime;
	if (TimeLived > TimeBeforeDestroy)
	{
		this->Destroy();
	}
}

