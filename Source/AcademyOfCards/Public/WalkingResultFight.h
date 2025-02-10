// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultFight : public WalkingResult
{
public:
	WalkingResultFight(TSharedPtr<FJsonObject> data);
	~WalkingResultFight();

protected:
	FString Opponent;
	TArray<TSharedPtr<WalkingResult>> ResultsWin;
	TArray<TSharedPtr<WalkingResult>> ResultsLose;
};
