// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultCard.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"
#include "UMyGameInstance.h"
#include <vector>

template <typename T>
TArray<T> shuffle(const TArray<T>& arr)
{
	std::vector<std::pair<int, int>> indexes;
	for (int i = 0; i < arr.Num(); ++i) indexes.push_back({ rand(), i});
	std::sort(indexes.begin(), indexes.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) { return a.second < b.second; });

	TArray<T> result;
	for (const auto& [weight, index] : indexes) {
		result.Add(arr[index]);
	}
	return result;
}

WalkingResultCard::WalkingResultCard(TSharedPtr<FJsonObject> data)
{
	IsGiveChoice = data->GetBoolField("give_choice");

	if (data->GetObjectField("how")->GetStringField("type") == "random_different") {
		How_Type = "random_different";
		How_Amount = data->GetObjectField("how")->GetNumberField("amount");
	} else if (data->GetObjectField("how")->GetStringField("type") == "random") {
		How_Type = "random";
		How_Amount = data->GetObjectField("how")->GetNumberField("amount");
	} else if (data->GetObjectField("how")->GetStringField("type") == "all") {
		How_Type = "all";
	} else {
		throw "Exception WalkingResultCard: no such how type " + data->GetObjectField("how")->GetStringField("type");
	}

	if (data->GetObjectField("from")->GetStringField("type") == "tag") {
		From_Type = "tag";
		for (TSharedPtr<FJsonValue> card : data->GetObjectField("from")->GetArrayField("tags")) {
			From_Pool.Add(card->AsString());
		}
	} else if (data->GetObjectField("from")->GetStringField("type") == "specific") {
		From_Type = "specific";
		for (TSharedPtr<FJsonValue> card : data->GetObjectField("from")->GetArrayField("ids")) {
			From_Pool.Add(card->AsString());
		}
	} else {
		throw "Exception WalkingResultCard: no such from type " + data->GetObjectField("from")->GetStringField("type");
	}

	if (data->HasField("result")) {
		for (auto& [x, y] : data->GetObjectField("result")->Values) {
			Results.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
		}
	}
}

WalkingResultCard::~WalkingResultCard()
{
}

void WalkingResultCard::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(player_model->GetGameInstance());

	TArray<FString> From_Pool_IDs;
	if (From_Type == "specific") {
		From_Pool_IDs = From_Pool;
	} else if (From_Type == "tag") {
		From_Pool_IDs = MyGameInstance->LoadedFightingConfigs->PullIdsByTags(From_Pool);
	}

	TArray<FString> ChosenIDs;
	if (How_Type == "random") {
		for (int i = 0; i < How_Amount; ++i) {
			ChosenIDs.Add(From_Pool_IDs[rand() % From_Pool_IDs.Num()]);
		}
	} else if (How_Type == "random_different") {
		From_Pool_IDs = shuffle(From_Pool_IDs);
		for (int i = 0; i < How_Amount && i < From_Pool_IDs.Num(); ++i) {
			ChosenIDs.Add(From_Pool_IDs[i]);
		}
	} else if (How_Type == "all") {
		ChosenIDs = From_Pool_IDs;
	}

	TArray<TSharedPtr<FightingCard>> ChosenCards;
	for (FString id : ChosenIDs) {
		ChosenCards.Add(MyGameInstance->LoadedFightingConfigs->GetCardByID(id));
	}

	if (IsGiveChoice) {
		walking_event->TextFromResult += "Choose a card from the list below\n";
		for (TSharedPtr<FightingCard> card : ChosenCards) {
			TArray<TSharedPtr<WalkingResult>> ResultAddSingleCard;
			ResultAddSingleCard.Add(FactoryCreateSingleCard(card, MyGameInstance));
			ResultAddSingleCard.Append(Results);
			walking_event->ButtonsFromResult.Add(std::make_pair("(" + card->ID + ") " + card->Name, ResultAddSingleCard));
		}
	} else {
		for (TSharedPtr<FightingCard> card : ChosenCards) {
			walking_event->TextFromResult += "You got a random card: (" + card->ID + ") " + card->Name + "\n";
			MyGameInstance->LoadedFightingDecks->AddToCurrentDeck(card->ID);
		}
	}
}

TSharedPtr<WalkingResultCard> WalkingResultCard::FactoryCreateSingleCard(TSharedPtr<FightingCard> Card, UUMyGameInstance* MyGameInstance)
{
	TSharedPtr<FJsonObject> data = MakeShareable(new FJsonObject());
	data->SetBoolField("give_choice", false);
	data->SetObjectField("how", MakeShareable(new FJsonObject()));
	data->GetObjectField("how")->SetStringField("type", "all");
	data->SetObjectField("from", MakeShareable(new FJsonObject()));
	data->GetObjectField("from")->SetStringField("type", "specific");
	TArray<TSharedPtr<FJsonValue>> tags;
	tags.Add(MakeShareable(new FJsonValueString(Card->ID)));
	data->GetObjectField("from")->SetArrayField("ids", tags);
	return MakeShareable(new WalkingResultCard(data));
}