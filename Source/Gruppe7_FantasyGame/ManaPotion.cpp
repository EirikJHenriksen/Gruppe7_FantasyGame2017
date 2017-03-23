// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "ManaPotion.h"


// Sets default values
AManaPotion::AManaPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision object and RootObject
	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MySphere"));
	RootComponent = RootSphere;
	//RootSphere->bGenerateOverlapEvents = true;
	//RootSphere->OnComponentBeginOverlap.AddDynamic(this, &AManaPotion::OnOverlap);

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

	++Rotator;
	SetActorRotation(FRotator(0.f, (1.f * Rotator), 0.f));
}

//void AManaPotion::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
//{
//  // Empty.
//}