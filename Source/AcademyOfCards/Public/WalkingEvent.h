// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class WalkingOption;

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingEvent
{
public:
	WalkingEvent(FString name, TSharedPtr<FJsonObject> data);
	~WalkingEvent();

protected:
	FString Name;
	FString Text;
	TArray<TSharedPtr<WalkingOption>> Options;
};
