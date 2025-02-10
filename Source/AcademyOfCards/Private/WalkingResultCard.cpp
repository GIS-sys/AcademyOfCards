// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultCard.h"

WalkingResultCard::WalkingResultCard(TSharedPtr<FJsonObject> data)
{
	for (TSharedPtr<FJsonValue> card : data->GetArrayField("options")) {
		CardOptions.Add(card->AsString());
	}
}

WalkingResultCard::~WalkingResultCard()
{
}
