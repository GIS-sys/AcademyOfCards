// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultFight.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultFight::WalkingResultFight(TSharedPtr<FJsonObject> data)
{
	Opponent = data->GetStringField("who");
	for (auto& [x, y] : data->GetObjectField("reward_win")->Values) {
		ResultsWin.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
	}
}

WalkingResultFight::~WalkingResultFight()
{
}

void WalkingResultFight::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	// TODO fight result
	walking_event->TextFromResult += "Fight: " + Opponent + "\n";
	walking_event->TextFromResult += "You won!\n";
	for (auto& Result : ResultsWin) {
		Result->Execute(walking_event, player_model);
	}
}