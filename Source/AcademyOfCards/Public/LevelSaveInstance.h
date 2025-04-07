// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API LevelSaveInstance
{
public:
	LevelSaveInstance();
	~LevelSaveInstance();

	TMap<FString, void*> Data;
};
