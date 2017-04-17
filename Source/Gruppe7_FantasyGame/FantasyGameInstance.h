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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVariables)
		int SpellSelect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerVariables)
		int CurrentLevel;

	FVector PlayerLocation;

	float GetHealth();

	float GetMana();

	void SetHealth(float Input);

	void SetMana(float Input);

	void DrainHealth(float Input);

	void DrainMana(float Input);

	void RestoreHealth(float Input);
	
	void RestoreMana(float Input);

	void SwapUp();

	void SwapDown();

	int GetCurrentSpell();
	
	void SetPlayerLocation(FVector Location);
	
	FVector GetPlayerLocation();

	void UFantasyGameInstance::SelectWater();

	void UFantasyGameInstance::SelectFire();

	void UFantasyGameInstance::SelectThorns();

	void UFantasyGameInstance::SelectHealing();

	void SetCurrentLevel(int level);

	int GetCurrentLevel();
};