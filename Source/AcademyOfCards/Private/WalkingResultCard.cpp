// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultCard.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultCard::WalkingResultCard(TSharedPtr<FJsonObject> data)
{
	if (data->HasField("options")) {
		for (TSharedPtr<FJsonValue> card : data->GetArrayField("options")) {
			CardOptions.Add(card->AsString());
		}
	}
	if (data->HasField("random")) {
		CardRandomAmount = data->GetObjectField("random")->GetNumberField("amount");
		for (TSharedPtr<FJsonValue> card : data->GetObjectField("random")->GetArrayField("tags")) {
			CardRandomTags.Add(card->AsString());
		}
	}
	if (data->HasField("specific")) {
		for (TSharedPtr<FJsonValue> card : data->GetArrayField("specific")) {
			CardSpecificIDs.Add(card->AsString());
		}
	}
}

WalkingResultCard::~WalkingResultCard()
{
}

void WalkingResultCard::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	// TODO card result
	for (auto& CardID : CardSpecificIDs) {
		walking_event->TextFromResult += "Card with ID: " + CardID + "\n";
	}
	for (int i = 0; i < CardRandomAmount; ++i) {
		walking_event->TextFromResult += "Random card with tags: ";
		for (FString Tag : CardRandomTags) {
			walking_event->TextFromResult += Tag + " ";
		}
		walking_event->TextFromResult += "\n";
	}
	if (!CardOptions.IsEmpty()) {
		walking_event->TextFromResult += "Choose a card from list below\n";
		for (auto& CardID : CardOptions) {
			//walking_event->OptionsFromResult.Add(); // TODO !!!
		}
	}
}