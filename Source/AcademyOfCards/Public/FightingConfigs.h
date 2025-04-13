// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <FightingAbility.h>
#include <FightingCard.h>

/**
 * 
 */
class ACADEMYOFCARDS_API FightingConfigs
{
protected:
	TArray<TSharedPtr<FightingAbility>> FightingAbilities;
	TArray<TSharedPtr<FightingCard>> FightingCards;

public:
	void LoadConfigAbilities();
	void LoadConfigCards();

	TSharedPtr<FightingCard> GetCardByID(FString ID) const;
};
