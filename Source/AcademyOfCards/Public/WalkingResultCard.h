// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
class UUMyGameInstance;
class UBUIWalkingEvent;
class ABActorWalkingPlayerModel;
class FightingCard;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultCard : public WalkingResult
{
public:
	WalkingResultCard(TSharedPtr<FJsonObject> data);
	~WalkingResultCard();

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);
	static TSharedPtr<WalkingResultCard> FactoryCreateSingleCard(TSharedPtr<FightingCard> Card, UUMyGameInstance* MyGameInstance);

protected:
	bool IsGiveChoice = false;
	FString How_Type;
	int How_Amount = 0;
	FString From_Type;
	TArray<FString> From_Pool;
	TArray<TSharedPtr<WalkingResult>> Results;
};
