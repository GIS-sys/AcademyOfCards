// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultStats : public WalkingResult
{
public:
	WalkingResultStats(TSharedPtr<FJsonObject> data);
	~WalkingResultStats();

protected:
	TSharedPtr<FJsonObject> StatsChange;
};
