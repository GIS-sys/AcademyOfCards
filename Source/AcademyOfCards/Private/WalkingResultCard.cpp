// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultCard.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"
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

	if (data->GetObjectField("how")->GetStringField("type") == "random") {
		How_Type = "random";
		How_Amount = data->GetObjectField("how")->GetNumberField("amount");
	}
	if (data->GetObjectField("how")->GetStringField("type") == "random_different") {
		How_Type = "random_different";
		How_Amount = data->GetObjectField("how")->GetNumberField("amount");
	}
	if (data->GetObjectField("how")->GetStringField("type") == "all") {
		How_Type = "all";
	}

	if (data->GetObjectField("from")->GetStringField("type") == "tag") {
		From_Type = "tag";
		for (TSharedPtr<FJsonValue> card : data->GetObjectField("from")->GetArrayField("tags")) {
			From_Pool.Add(card->AsString());
		}
	}
	if (data->GetObjectField("from")->GetStringField("type") == "specific") {
		From_Type = "specific";
		for (TSharedPtr<FJsonValue> card : data->GetObjectField("from")->GetArrayField("ids")) {
			From_Pool.Add(card->AsString());
		}
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
	// TODO card result
	TArray<FString> Chosen;

	if (How_Type == "random") {
		for (int i = 0; i < How_Amount; ++i) {
			Chosen.Add(From_Pool[rand() % From_Pool.Num()]);
		}
	}
	if (How_Type == "random_different") {
		From_Pool = shuffle(From_Pool);
		for (int i = 0; i < How_Amount && i < From_Pool.Num(); ++i) {
			Chosen.Add(From_Pool[i]);
		}
	}
	if (How_Type == "all") {
		Chosen = From_Pool;
	}

	TArray<CardType> ChosenCards;

	if (From_Type == "tag") {
		for (FString tag : Chosen) {
			ChosenCards.Add("(tag:" + tag + ")");
		}
	}
	if (From_Type == "specific") {
		for (FString id : Chosen) {
			ChosenCards.Add("(id:" + id + ")");
		}
	}

	if (IsGiveChoice) {
		walking_event->TextFromResult += "Choose a card from the list below\n";
		for (CardType card : ChosenCards) {
			TArray<TSharedPtr<WalkingResult>> ResultAddSingleCard;
			ResultAddSingleCard.Add(FactoryCreateSingleCard(card));
			ResultAddSingleCard.Append(Results);
			// ResultAddSingleCard.Add(WalkingResult::FactoryCreate("__close__", nullptr));
			walking_event->ButtonsFromResult.Add(std::make_pair(card, ResultAddSingleCard));
		}
	} else {
		for (FString card : ChosenCards) {
			walking_event->TextFromResult += "Random card: " + card + "\n";
		}
	}
}

TSharedPtr<WalkingResultCard> WalkingResultCard::FactoryCreateSingleCard(CardType Card)
{
	TSharedPtr<FJsonObject> data = MakeShareable(new FJsonObject());
	data->SetBoolField("give_choice", false);
	data->SetObjectField("how", MakeShareable(new FJsonObject()));
	data->GetObjectField("how")->SetStringField("type", "all");
	data->SetObjectField("from", MakeShareable(new FJsonObject()));
	data->GetObjectField("from")->SetStringField("type", "tag");
	TArray<TSharedPtr<FJsonValue>> tags;
	tags.Add(MakeShareable(new FJsonValueString(Card)));
	data->GetObjectField("from")->SetArrayField("tags", tags);
	return MakeShareable(new WalkingResultCard(data));
}