// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingDeck.h"
#include "WalkingEvent.h"
#include "WalkingCardConfig.h"

WalkingDeck::WalkingDeck()
{
}

WalkingDeck::~WalkingDeck()
{
}

void WalkingDeck::LoadConfigEvents() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/config_walking_events_new.json");
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

void WalkingDeck::LoadConfigCards() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/event_cards_config_new.json");
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

TSharedPtr<WalkingCardConfig> WalkingDeck::GetRandomCard() const {
	return CardConfigs[FMath::Rand() % CardConfigs.Num()];
}

TSharedPtr<WalkingEvent> WalkingDeck::GetEventByName(FString Name) const {
	for (const auto& event : Events) {
		if (event->Name == Name) return event;
	}
	return WalkingEvent::CreateDefault();
}

TSharedPtr<WalkingEvent> WalkingDeck::GetEventByID(FString ID) const {
	for (const auto& event : Events) {
		if (event->ID == ID) return event;
	}
	return WalkingEvent::CreateDefault();
}