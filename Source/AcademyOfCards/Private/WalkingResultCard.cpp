// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultCard.h"

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
