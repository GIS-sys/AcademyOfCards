// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class WalkingResult;

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingOption
{
public:
	WalkingOption(TSharedPtr<FJsonObject> data);
	~WalkingOption();

public:
	FString Text;
	TArray<TSharedPtr<WalkingResult>> Results;
};
