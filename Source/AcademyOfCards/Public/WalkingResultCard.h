// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
class UBUIWalkingEvent;
class ABActorWalkingPlayerModel;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultCard : public WalkingResult
{
public:
	WalkingResultCard(TSharedPtr<FJsonObject> data);
	~WalkingResultCard();

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);

protected:
	bool IsGiveChoice;
	FString How_Type;
	int How_Amount;
	FString From_Type;
	TArray<FString> From_Pool;
};
