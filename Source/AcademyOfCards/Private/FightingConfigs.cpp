// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingConfigs.h"

void FightingConfigs::LoadConfigAbilities() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/abilities.json");
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		for (const auto& value : JsonArray) {
			FightingAbilities.Add(MakeShareable(new FightingAbility(value->AsObject())));
		}
	}
}

void FightingConfigs::LoadConfigCards() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/cards.json");
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
		return;
	}

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		for (const auto& value : JsonArray) {
			FightingCards.Add(MakeShareable(new FightingCard(value->AsObject())));
		}
	}
}

TSharedPtr<FightingCard> FightingConfigs::GetCardByID(FString ID) const {
	for (auto& card : FightingCards) {
		if (card->ID == ID) return card;
	}
	return nullptr;
}