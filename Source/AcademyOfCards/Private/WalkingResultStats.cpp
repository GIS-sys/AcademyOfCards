// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultStats.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultStats::WalkingResultStats(TSharedPtr<FJsonObject> data)
{
	StatsChange = data;
}

WalkingResultStats::~WalkingResultStats()
{
}

void WalkingResultStats::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	if (StatsChange->HasField("gold")) {
		player_model->PlayerStats.Gold += StatsChange->GetNumberField("gold");
		walking_event->TextFromResult += "Gold: " + FString::SanitizeFloat(StatsChange->GetNumberField("gold")) + "\n";
	}
	if (StatsChange->HasField("health")) {
		player_model->PlayerStats.Health += StatsChange->GetNumberField("health");
		walking_event->TextFromResult += "Health: " + FString::SanitizeFloat(StatsChange->GetNumberField("health")) + "\n";
	}
}
