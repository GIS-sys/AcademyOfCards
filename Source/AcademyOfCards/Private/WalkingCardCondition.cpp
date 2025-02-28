// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingCardCondition.h"

WalkingCardCondition::WalkingCardCondition(TSharedPtr<FJsonObject> config)
{
	EventID = config->GetStringField("id");
	if (config->HasField("condition")) {
		for (const auto& [condition_target, condition_clause] : config->GetObjectField("condition")->Values) {
			Conditions.Add(TPair<FString, TSharedPtr<FJsonObject>>(condition_target, condition_clause->AsObject()));
		}
	}
}

WalkingCardCondition::~WalkingCardCondition()
{
}

bool WalkingCardCondition::IsFired(const FPlayerStats& stats) const {
	if (Conditions.IsEmpty()) return true;
	for (const auto& condition : Conditions) {
		if (FMath::Rand() % Conditions.Num() == 0) return true; // TODO actually check condition
	}
	return false;
}