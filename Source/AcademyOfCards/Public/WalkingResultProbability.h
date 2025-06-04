// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
class UBUIWalkingEvent;
class ABActorWalkingPlayerModel;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultProbability: public WalkingResult
{
public:
	WalkingResultProbability(TSharedPtr<FJsonObject> data);
	~WalkingResultProbability();

	struct WalkingResultProbabilityInner {
		FString Name;
		FString Probability;
		TArray<TSharedPtr<WalkingResult>> Results;

		float Evaluate(ABActorWalkingPlayerModel* player_model) const;
	};

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);

protected:
	TArray<WalkingResultProbabilityInner> ProbabilityInners;
};
