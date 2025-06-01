// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>
#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API DeckLists
{
protected:
	std::map<FString, TArray<FString>> LoadedDecksIDs;
	TArray<FString> CurrentPlayerDeckIDs;

public:
	static const char* START_DECK;
	static const char* DEFAULT_NAME;

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

	TArray<FString> GetDeckIDsByName(FString Name);
	TArray<FString> GetDeckIDsCurrentPlayer();
};
