// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingConfigs.h"

void FightingConfigs::LoadConfigAbilities() {
	// TODOIMPORTANT
}

void FightingConfigs::LoadConfigCards() {
	// TODOIMPORTANT
}

TSharedPtr<FightingCard> FightingConfigs::GetCardByID(FString ID) const {
	for (auto& card : FightingCards) {
		if (card->ID == ID) return card;
	}
	return nullptr;
}