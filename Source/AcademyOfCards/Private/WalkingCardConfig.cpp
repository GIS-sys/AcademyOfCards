// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingCardConfig.h"
#include "WalkingCardCondition.h"

WalkingCardConfig::WalkingCardConfig(TSharedPtr<FJsonObject> config)
{
	Name = config->GetStringField("name");
	for (const auto& condition : config->GetArrayField("conditions")) {
		Conditions.Add(MakeShareable(new WalkingCardCondition(condition->AsObject())));
	}
}

WalkingCardConfig::~WalkingCardConfig()
{
}

FString WalkingCardConfig::GetEventFired(const FPlayerStats& stats) const {
	for (const auto& condition : Conditions) {
		if (condition->IsFired(stats)) return condition->EventName;
	}
	return "";
}