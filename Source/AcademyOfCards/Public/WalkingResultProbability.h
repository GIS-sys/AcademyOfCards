// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

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
		FString Probability; // TODO parse somehow
		TArray<TSharedPtr<WalkingResult>> Results;
	};

protected:
	TArray<WalkingResultProbabilityInner> ProbabilityInners;
};
