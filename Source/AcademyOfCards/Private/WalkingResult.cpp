// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResult.h"
#include <WalkingResultAlignment.h>
#include <WalkingResultCard.h>
#include <WalkingResultFight.h>
#include <WalkingResultProbability.h>
#include <WalkingResultResponse.h>
#include <WalkingResultStats.h>

WalkingResult::WalkingResult()
{
}

WalkingResult::~WalkingResult()
{
}

TSharedPtr<WalkingResult> WalkingResult::FactoryCreate(FString name, TSharedPtr<FJsonObject> data)
{
	if (name == "response") {
		return MakeShareable(new WalkingResultResponse(data));
	}
	if (name == "fight") {
		return MakeShareable(new WalkingResultFight(data));
	}
	if (name == "stats") {
		return MakeShareable(new WalkingResultStats(data));
	}
	if (name == "card") {
		return MakeShareable(new WalkingResultCard(data));
	}
	if (name == "probability") {
		return MakeShareable(new WalkingResultProbability(data));
	}
	if (name == "alignment") {
		return MakeShareable(new WalkingResultAlignment(data));
	}
	return MakeShareable<WalkingResult>(nullptr);
}