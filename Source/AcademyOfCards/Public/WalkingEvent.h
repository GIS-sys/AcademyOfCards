// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class WalkingOption;

#include "BActorWalkingDealer.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingEvent
{
public:
	WalkingEvent(FString name, FString text, TArray<TSharedPtr<WalkingOption>> options);
	WalkingEvent(FString name, TSharedPtr<FJsonObject> data);
	~WalkingEvent();

	void Fire(ABActorWalkingDealer* DealerPtr);
	static TSharedPtr<WalkingEvent> CreateDefault();

	FString Name;

protected:
	FString Text;
	TArray<TSharedPtr<WalkingOption>> Options;
};
