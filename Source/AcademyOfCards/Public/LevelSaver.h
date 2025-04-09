// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSaveInstance.h"

/**
 * 
 */
class ACADEMYOFCARDS_API LevelSaver
{
public:
	LevelSaver() = default;
	~LevelSaver() = default;

	TMap<FString, LevelSaveInstance> Saves;
};
