// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMyGameInstance.h"
#include "LevelSaveInstance.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingDeck
{
protected:
	TArray<FString> CardIDs;

public:
	WalkingDeck(UUMyGameInstance* GameInstance);
	~WalkingDeck();

	UUMyGameInstance* MyGameInstance;

	TSharedPtr<WalkingCardConfig> GetRandomCard() const;

	LevelSaveInstance Save() { return LevelSaveInstance(); };
	void Load(LevelSaveInstance* SaveInstance) {};
};
