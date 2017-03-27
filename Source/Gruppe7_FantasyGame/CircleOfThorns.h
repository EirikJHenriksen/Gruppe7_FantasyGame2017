// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CircleOfThorns.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API ACircleOfThorns : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACircleOfThorns();

	UPROPERTY(EditAnywhere)
		UShapeComponent* RootSphere;

	// TEST.
	FVector CurrentVelocity;
	bool Stopped = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float TimeBeforeDestroy = 10.f;

	float TimeLived{ 0 };

};