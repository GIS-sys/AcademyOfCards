// Fill out your copyright notice in the Description page of Project Settings.


#include "DeckLists.h"


const char* DeckLists::START_DECK = "Start_deck";
const char* DeckLists::DEFAULT_NAME = "Basic_opponent";

void DeckLists::Load() {
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *Path)) {
		UE_LOG(LogTemp, Error, TEXT("DeckLists: Failed to load JSON file from path: %s"), *Path);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonArray)) {
		for (const auto& value : JsonArray) {
			for (const auto [name, raw_ids] : value->AsObject()->Values) {
				TArray<FString> ids;
				for (const auto& raw_id : raw_ids->AsArray()) {
					ids.Add(raw_id->AsString());
				}
				LoadedDecksIDs[name] = ids;
			}
		}
	}

	CurrentPlayerDeckIDs = GetDeckIDsByName(START_DECK);
}

TArray<FString> DeckLists::GetDeckIDsByName(FString Name) {
	if (LoadedDecksIDs.find(Name) != LoadedDecksIDs.end()) {
		return LoadedDecksIDs.find(Name)->second;
	}
	if (LoadedDecksIDs.find(DEFAULT_NAME) != LoadedDecksIDs.end()) {
		return LoadedDecksIDs.find(DEFAULT_NAME)->second;
	}
	return TArray<FString>();
}

TArray<FString> DeckLists::GetDeckIDsCurrentPlayer() {
	return CurrentPlayerDeckIDs;
}

void DeckLists::AddToCurrentDeck(const FString& id) {
	CurrentPlayerDeckIDs.Add(id);
}