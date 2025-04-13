// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UUMyGameInstance;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbility
{
public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	~FightingAbility();

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	FightingAbility Build(TSharedPtr<FJsonObject> Arguments);
};
