// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <FightingAbility.h>
#include <FightingCard.h>

class UUMyGameInstance;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingConfigs
{
protected:
	TArray<TSharedPtr<FightingAbility>> FightingAbilities;
	TArray<TSharedPtr<FightingCard>> FightingCards;

public:
	void LoadConfigAbilities(UUMyGameInstance* MyGameInstance);
	void LoadConfigCards(UUMyGameInstance* MyGameInstance);

	TSharedPtr<FightingCard> GetCardByID(FString ID) const;
	TSharedPtr<FightingAbility> GetAbilityByID(FString ID) const;

	TArray<FString> PullIdsByTags(const TArray<FString>& Tags) const;
};
