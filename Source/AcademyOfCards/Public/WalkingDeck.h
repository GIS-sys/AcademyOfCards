// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class WalkingEvent;
class WalkingCardConfig;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingDeck
{
protected:
	TArray<TSharedPtr<WalkingEvent>> Events;
	TArray<TSharedPtr<WalkingCardConfig>> CardConfigs;

public:
	WalkingDeck();
	~WalkingDeck();

	void LoadConfigEvents();
	void LoadConfigCards();

	TSharedPtr<WalkingCardConfig> GetRandomCard() const;
	TSharedPtr<WalkingCardConfig> GetCardByID(FString ID) const;
	TSharedPtr<WalkingEvent> GetEventByName(FString Name) const;
	TSharedPtr<WalkingEvent> GetEventByID(FString ID) const;
};
