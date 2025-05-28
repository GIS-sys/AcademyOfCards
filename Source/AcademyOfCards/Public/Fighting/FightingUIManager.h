// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABActorFightingField;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingUIManager
{
	ABActorFightingField* Field = nullptr;

public:
	FightingUIManager();
	~FightingUIManager();

	void Init(ABActorFightingField* NewField);
};
