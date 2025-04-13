// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatStructs.h"
#include "FightingAbility.h"
#include <FightingUnitParameters.h>
class UUMyGameInstance;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingCard
{
public:
	FightingCard(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	~FightingCard();

	FString ID;
    FString Name;
    FString Cardtext;
    FString Type;
    FString AlignmentKind;
    FMana ManaCost;
    UFightingUnitParameters* UnitParameters;
};
