// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <LevelSaveInstance.h>
#include "UMyGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EnumStage : uint8 {
	WALKING = 0,
	FIGHTING = 1
};

UCLASS()
class ACADEMYOFCARDS_API UUMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	LevelSaveInstance WalkingSave;

	LevelSaveInstance FightingSave;

	UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
	bool NotLoadedWalkingBefore = false; // TODO

	UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
	EnumStage PersistentStage = EnumStage::WALKING;
};
