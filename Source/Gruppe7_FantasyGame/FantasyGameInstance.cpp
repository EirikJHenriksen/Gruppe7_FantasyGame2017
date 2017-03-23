// Fill out your copyright notice in the Description page of Project Settings.

#include "Gruppe7_FantasyGame.h"
#include "Gruppe7_FantasyGameCharacter.h"
#include "FantasyGameInstance.h"

// Sets default values
UFantasyGameInstance::UFantasyGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Health = 1.f;
	Mana = 1.f;
}

float UFantasyGameInstance::GetHealth()
{
	return Health;
}

float UFantasyGameInstance::GetMana()
{
	return Mana;
}

void UFantasyGameInstance::SetHealth(float Input)
{
	Health = Input;
}

void UFantasyGameInstance::SetMana(float Input)
{
	Mana = Input;
}

void UFantasyGameInstance::DrainHealth(float Input)
{
	Health -= Input;
}

void UFantasyGameInstance::DrainMana(float Input)
{
	Mana -= Input;
}

void UFantasyGameInstance::RestoreHealth(float Input)
{
	Health += Input;
	// Sørger for at Health ikke går over 100%.
	if (Health > 1.f)
	{
		Health = 1.f;
	}
}

void UFantasyGameInstance::RestoreMana(float Input)
{
	Mana += Input;
	// Sørger for at Mana ikke går over 100%.
	if (Mana > 1.f)
	{
		Mana = 1.f;
	}
}

void UFantasyGameInstance::SwapUp()
{
	++SpellSelect;

	if (SpellSelect == 4)
	{
		SpellSelect = 0;
	}
}

void UFantasyGameInstance::SwapDown()
{
	--SpellSelect;

	if (SpellSelect == -1)
	{
		SpellSelect = 3;
	}
}

int UFantasyGameInstance::GetCurrentSpell()
{
	return SpellSelect;
}

void UFantasyGameInstance::SetPlayerLocation(FVector Location)
{
	PlayerLocation = Location;
}

FVector UFantasyGameInstance::GetPlayerLocation()
{
	return PlayerLocation;
}