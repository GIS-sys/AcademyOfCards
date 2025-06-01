// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <LevelSaver.h>
#include "WalkingConfigs.h"
#include <FightingConfigs.h>
#include <DeckLists.h>
#include "UMyGameInstance.generated.h"

class WalkingDeck;
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
	// Saves
	static const FString SAVE_WALKING_PLAYER_STATS;
	static const FString SAVE_WALKING_FIGHT_RESULT;
	static const FString SAVE_WALKING_DEALER;
	static const FString SAVE_WALKING_PLAYER_MODEL;
	static const FString SAVE_FIGHTING_PLAYER_STATS;
	static const FString SAVE_FIGHTING_FIGHT_OUTCOME;

	LevelSaver WalkingSave;
	LevelSaver FightingSave;

	UFUNCTION(BlueprintCallable, Category = "Persistent Data")
	bool HasWalkingSave() const {
		return !WalkingSave.Saves.IsEmpty();
	}
	UFUNCTION(BlueprintCallable, Category = "Persistent Data")
	bool HasFightingSave() const {
		return !FightingSave.Saves.IsEmpty();
	}

	UPROPERTY(BlueprintReadWrite, Category = "Persistent Data")
	EnumStage PersistentStage = EnumStage::WALKING;

	// Configs
	TSharedPtr<WalkingConfigs> LoadedWalkingConfigs;
	TSharedPtr<FightingConfigs> LoadedFightingConfigs;
	TSharedPtr<DeckLists> LoadedWalkingDecks;
	TSharedPtr<DeckLists> LoadedFightingDecks;
	virtual void Init() override;
};
