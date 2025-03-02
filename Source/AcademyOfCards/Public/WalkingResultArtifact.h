// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
class UBUIWalkingEvent;
class ABActorWalkingPlayerModel;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultArtifact : public WalkingResult
{
public:
	WalkingResultArtifact(TSharedPtr<FJsonObject> data);
	~WalkingResultArtifact();

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);

	FString ID;
};
