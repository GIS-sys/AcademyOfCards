// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingConfigs.h"
#include "WalkingEvent.h"
#include "WalkingCardConfig.h"

void WalkingConfigs::LoadConfigEvents() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/events.json");
	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			for (const auto& [x, y] : JsonObject->Values) {
				Events.Add(MakeShareable(new WalkingEvent(x, y->AsObject())));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
	}
}

void WalkingConfigs::LoadConfigCards() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/cards.json");
	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		TArray<TSharedPtr<FJsonValue>> JsonArray;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			for (const auto& card_value : JsonArray) {
				CardConfigs.Add(MakeShareable(new WalkingCardConfig(card_value->AsObject())));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
	}
}

TSharedPtr<WalkingEvent> WalkingConfigs::GetEventByName(FString Name) const {
	for (const auto& event : Events) {
		if (event->Name == Name) return event;
	}
	return WalkingEvent::CreateDefault();
}

TSharedPtr<WalkingEvent> WalkingConfigs::GetEventByID(FString ID) const {
	for (const auto& event : Events) {
		if (event->ID == ID) return event;
	}
	return WalkingEvent::CreateDefault();
}

TSharedPtr<WalkingCardConfig> WalkingConfigs::GetCardByID(FString ID) const {
	for (auto& card : CardConfigs) {
		if (card->ID == ID) return card;
	}
	return nullptr;
}