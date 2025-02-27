// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorWalkingPlayerModel.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingCardCondition
{
protected:
	TArray<TSharedPtr<FJsonObject>> Conditions;

public:
	WalkingCardCondition(TSharedPtr<FJsonObject> config);
	~WalkingCardCondition();

	bool IsFired(const FPlayerStats& stats) const;

	FString EventID;
};
