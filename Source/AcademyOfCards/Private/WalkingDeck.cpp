// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingDeck.h"
#include "WalkingEvent.h"
#include "WalkingCardConfig.h"

WalkingDeck::WalkingDeck(UUMyGameInstance* GameInstance) : MyGameInstance(GameInstance)
{
	CardIDs = { "001u", "002u", "003u", "001c", "002c" };
}

WalkingDeck::~WalkingDeck()
{
}

TSharedPtr<WalkingCardConfig> WalkingDeck::GetRandomCard() const {
	FString RandomId = CardIDs[FMath::Rand() % CardIDs.Num()];
	return MyGameInstance->LoadedWalkingConfigs->GetCardByID(RandomId);
}
