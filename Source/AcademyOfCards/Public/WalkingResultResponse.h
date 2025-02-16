// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultResponse : public WalkingResult
{
public:
	WalkingResultResponse(TSharedPtr<FJsonObject> data);
	virtual ~WalkingResultResponse();

	virtual void Execute();

protected:
	FString Response;
};
