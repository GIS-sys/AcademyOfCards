// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
class UBUIWalkingEvent;
class ABActorWalkingPlayerModel;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultStats : public WalkingResult
{
public:
	WalkingResultStats(TSharedPtr<FJsonObject> data);
	~WalkingResultStats();

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);

protected:
	TSharedPtr<FJsonObject> StatsChange;
};
