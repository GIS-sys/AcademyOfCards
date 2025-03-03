// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultProbability.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultProbability::WalkingResultProbability(TSharedPtr<FJsonObject> data)
{
	for (auto& [name, inner_data] : data->Values) {
		auto inners = WalkingResultProbability::WalkingResultProbabilityInner{ .Name = name, .Probability = inner_data->AsObject()->GetStringField("probability")};
		for (auto& [x, y] : inner_data->AsObject()->GetObjectField("result")->Values) {
			inners.Results.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
		}
		ProbabilityInners.Add(inners);
	}
}

WalkingResultProbability::~WalkingResultProbability()
{
}

void WalkingResultProbability::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	// TODO parse somehow probability result
	WalkingResultProbabilityInner ChosenInner = ProbabilityInners[rand() % ProbabilityInners.Num()];
	walking_event->TextFromResult += "Random chosen: " + ChosenInner.Name + "\n";
	for (auto& result : ChosenInner.Results) {
		result->Execute(walking_event, player_model);
	}
}