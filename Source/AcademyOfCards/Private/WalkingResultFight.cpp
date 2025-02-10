// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultFight.h"

WalkingResultFight::WalkingResultFight(TSharedPtr<FJsonObject> data)
{
	Opponent = data->GetStringField("who");
	for (auto& [x, y] : data->GetObjectField("reward_win")->Values) {
		ResultsWin.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
	}
	for (auto& [x, y] : data->GetObjectField("reward_lose")->Values) {
		ResultsLose.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
	}
}

WalkingResultFight::~WalkingResultFight()
{
}
