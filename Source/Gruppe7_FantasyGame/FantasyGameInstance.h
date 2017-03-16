// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "FantasyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GRUPPE7_FANTASYGAME_API UFantasyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Sets default values for main character.

	UFantasyGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVariables)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVariables)
		float Mana;


	float GetHealth();

	float GetMana();

	void SetHealth(float Input);

	void SetMana(float Input);

	void DrainHealth(float Input);

	void DrainMana(float Input);

	void RestoreHealth(float Input);
	
	void RestoreMana(float Input);
};
