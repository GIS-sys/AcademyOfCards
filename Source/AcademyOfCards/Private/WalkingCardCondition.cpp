// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingCardCondition.h"

WalkingCardCondition::WalkingCardCondition(TSharedPtr<FJsonObject> config)
{
	EventID = config->GetStringField("id");
	if (config->HasField("condition")) {
		for (const auto& condition_clause : config->GetArrayField("condition")) {
			Conditions.Add(condition_clause->AsObject());
		}
	}
}

WalkingCardCondition::~WalkingCardCondition()
{
}

bool WalkingCardCondition::IsFired(const FPlayerStats& stats) const {
	if (Conditions.IsEmpty()) return true;
	for (const auto& condition : Conditions) {
		// TODO
	}
	return false;
}