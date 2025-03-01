// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResult.h"
#include <WalkingResultAlignment.h>
#include <WalkingResultCard.h>
#include <WalkingResultFight.h>
#include <WalkingResultProbability.h>
#include <WalkingResultResponse.h>
#include <WalkingResultSpecialClose.h>
#include <WalkingResultStats.h>
#include <WalkingResultArtifact.h>

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
	if (name == "artifact") {
		return MakeShareable(new WalkingResultArtifact(data));
	}
	if (name == "__close__") {
		return MakeShareable(new WalkingResultSpecialClose(data));
	}
	throw "Exception at FactoryCreate WalkingResult: no such " + name + " is available";
}

void WalkingResult::Execute()
{
	UE_LOG(LogTemp, Error, TEXT("EXECUTE: not implemented"));
}