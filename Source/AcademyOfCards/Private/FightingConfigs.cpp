// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingConfigs.h"
#include "UMyGameInstance.h"

void FightingConfigs::LoadConfigAbilities(UUMyGameInstance* MyGameInstance) {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("FightStage/Configs/abilities.json");
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
			FightingAbilities.Add(MakeShareable(new FightingAbility(value->AsObject(), MyGameInstance)));
		}
	}
}

void FightingConfigs::LoadConfigCards(UUMyGameInstance* MyGameInstance) {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("FightStage/Configs/cards.json");
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
			FightingCards.Add(MakeShareable(new FightingCard(value->AsObject(), MyGameInstance)));
		}
	}
}

TSharedPtr<FightingCard> FightingConfigs::GetCardByID(FString ID) const {
	for (auto& card : FightingCards) {
		if (card->ID == ID) return card;
	}
	return nullptr;
}

TSharedPtr<FightingAbility> FightingConfigs::GetAbilityByID(FString ID) const {
	for (auto& ability : FightingAbilities) {
		if (ability->ID == ID) return ability;
	}
	return nullptr;
}

TArray<FString> FightingConfigs::PullIdsByTags(const TArray<FString>& Tags) const {
	TArray<FString> IDs;
	for (FString tag : Tags) {
		for (const auto& card : FightingCards) {
			if (card->AlignmentKind == "Fire" && tag == "fire")
				IDs.Add(card->ID);
			else if (card->AlignmentKind == "Ice" && tag == "ice")
				IDs.Add(card->ID);
			else if (card->AlignmentKind == "Light" && tag == "light")
				IDs.Add(card->ID);
			else if (card->AlignmentKind == "Dark" && tag == "dark")
				IDs.Add(card->ID);
			else if ((card->Rarity == "Common" || card->Rarity == "") && tag == "common")
				IDs.Add(card->ID);
			else if (card->Rarity == "Rare" && tag == "rare")
				IDs.Add(card->ID);
			else if (card->Rarity == "Legendary" && tag == "legendary")
				IDs.Add(card->ID);
			// TODO IMPORTANT
		}
	}
	return IDs;
}