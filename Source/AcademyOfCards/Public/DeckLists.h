// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API DeckLists
{
protected:
	TMap<FString, TArray<FString>> LoadedDecks;

public:
	enum TYPE : uint8 {
		WALKING = 0,
		FIGHTING,
	};

	TYPE Type;
	FString Path;

	DeckLists(TYPE Type) : Type(Type) {
		if (Type == TYPE::WALKING) {
			Path = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/decks.json");
		} else if (Type == TYPE::FIGHTING) {
			Path = FPaths::ProjectContentDir() / TEXT("FightStage/Configs/decks.json");
		} else {
			throw std::exception("DeckLists got weird Type");
		}
	}

	void Load();
};
