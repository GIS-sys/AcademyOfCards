// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorWalkingPlayerModel.h"
class WalkingCardCondition;
class WalkingCardCondition;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingCardConfig
{
protected:
	TArray<TSharedPtr<WalkingCardCondition>> Conditions;

public:
	WalkingCardConfig(TSharedPtr<FJsonObject> config);
	~WalkingCardConfig();

	FString GetEventFired(const FPlayerStats& stats) const;

	FString Name;
};
