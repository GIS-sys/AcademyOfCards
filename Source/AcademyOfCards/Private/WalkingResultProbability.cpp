// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultProbability.h"

WalkingResultProbability::WalkingResultProbability(TSharedPtr<FJsonObject> data)
{
	for (auto& [name, inner_data] : data->Values) {
		auto inners = WalkingResultProbability::WalkingResultProbabilityInner{ .Name = name, .Probability = inner_data->AsObject()->GetNumberField("probability")};
		for (auto& [x, y] : inner_data->AsObject()->GetObjectField("result")->Values) {
			inners.Results.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
		}
		ProbabilityInners.Add(inners);
	}
}

WalkingResultProbability::~WalkingResultProbability()
{
}
