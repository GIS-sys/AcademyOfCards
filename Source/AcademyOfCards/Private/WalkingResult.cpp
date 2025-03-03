// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResult.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

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
	if (name == "response") { // UBUIWalkingEvent -> text?
		return MakeShareable(new WalkingResultResponse(data));
	}
	if (name == "fight") { // inner ...
		return MakeShareable(new WalkingResultFight(data));
	}
	if (name == "stats") { // ABActorWalkingPlayerModel -> PlayerStats
		return MakeShareable(new WalkingResultStats(data));
	}
	if (name == "card") { // for now UBUIWalkingEvent -> text?
		return MakeShareable(new WalkingResultCard(data));
	}
	if (name == "probability") { // inner ...
		return MakeShareable(new WalkingResultProbability(data));
	}
	if (name == "alignment") { // ABActorWalkingPlayerModel -> PlayerStats
		return MakeShareable(new WalkingResultAlignment(data));
	}
	if (name == "artifact") { // ABActorWalkingPlayerModel -> Artifacts?
		return MakeShareable(new WalkingResultArtifact(data));
	}
	if (name == "__close__") { // UBUIWalkingEvent -> NewEventPopup_Clear()
		return MakeShareable(new WalkingResultSpecialClose(data));
	}
	throw "Exception at FactoryCreate WalkingResult: no such " + name + " is available";
}

void WalkingResult::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	UE_LOG(LogTemp, Error, TEXT("EXECUTE: not implemented"));
}