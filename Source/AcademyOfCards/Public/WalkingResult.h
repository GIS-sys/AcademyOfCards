// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResult
{
public:
	WalkingResult();
	~WalkingResult();

	static TSharedPtr <WalkingResult> FactoryCreate(FString name, TSharedPtr<FJsonObject> data);
};
