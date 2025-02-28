// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultAlignment : public WalkingResult
{
public:
	WalkingResultAlignment(TSharedPtr<FJsonObject> data);
	~WalkingResultAlignment();

protected:
	float DarkAlignment = 0;
	float LightAlignment = 0;
	float IceAlignment = 0;
	float FireAlignment = 0;
};
