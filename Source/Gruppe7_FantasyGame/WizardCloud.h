// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WizardCloud.generated.h"

UCLASS()
class GRUPPE7_FANTASYGAME_API AWizardCloud : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWizardCloud();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// SPAWN LOCATION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Cloud SPAWN")
		float SpawnX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Cloud SPAWN")
		float SpawnY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Cloud SPAWN")
		float SpawnZ = 0;

	UPROPERTY(EditAnywhere)
		UShapeComponent* RootSphere;
	
};
