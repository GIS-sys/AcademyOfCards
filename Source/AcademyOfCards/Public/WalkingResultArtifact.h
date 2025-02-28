// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultArtifact : public WalkingResult
{
public:
	WalkingResultArtifact(TSharedPtr<FJsonObject> data);
	~WalkingResultArtifact();

	FString ID;
};
