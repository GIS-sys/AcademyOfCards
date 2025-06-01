// Fill out your copyright notice in the Description page of Project Settings.


#include "DeckLists.h"


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
				LoadedDecks.Add(name, ids);
			}
		}
	}
}