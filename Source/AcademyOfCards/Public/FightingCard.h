// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API FightingCard
{
public:
	FightingCard(TSharedPtr<FJsonObject> data);
	~FightingCard();

	// TODOIMPORTANT
	const FString ID = "";
};
