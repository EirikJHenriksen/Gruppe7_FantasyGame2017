// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Enemy_AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class GRUPPE7_FANTASYGAME_API AEnemy_AI_Controller : public AAIController
{
	GENERATED_BODY()

	//	UPROPERTY(transient)
	//	class UBlackboardComponent *BlackboardComp;

	//UPROPERTY(transient)
	//	class UBehaviorTreeComponent *BehaviorComp;
	
public:
	AEnemy_AI_Controller();

	virtual void Possess(APawn *InPawn) override;

	//uint8 EnemyKeyID;

	virtual void Tick(float DeltaTime) override;

	FVector MyHome;

};
