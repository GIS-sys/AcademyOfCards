// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultCard : public WalkingResult
{
public:
	WalkingResultCard(TSharedPtr<FJsonObject> data);
	~WalkingResultCard();

protected:
	TArray<FString> CardOptions;
};
