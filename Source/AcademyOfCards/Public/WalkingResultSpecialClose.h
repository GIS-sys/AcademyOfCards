// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>

/**
 *
 */
class ACADEMYOFCARDS_API WalkingResultSpecialClose : public WalkingResult
{
public:
	WalkingResultSpecialClose(TSharedPtr<FJsonObject> data);
	~WalkingResultSpecialClose();
};
