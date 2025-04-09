// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <WalkingResult.h>
#include <BUIWalkingEvent.h>
#include "LevelSaveInstance.h"
class ABActorWalkingPlayerModel;

/**
 * 
 */
class ACADEMYOFCARDS_API WalkingResultFight : public WalkingResult
{
public:
	WalkingResultFight(TSharedPtr<FJsonObject> data);
	~WalkingResultFight();

	virtual void Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);
	void ExecuteAfterFight(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model);

	FString GetOpponent() const {
		return Opponent;
	}

	LevelSaveInstance Save() {
		LevelSaveInstance SaveInstance;
		SaveInstance.Set(LevelSaveInstance::DEFAULT_NAME, new WalkingResultFight(*this));
		return SaveInstance;
	}

	static WalkingResultFight* Load(LevelSaveInstance* SaveInstance) {
		return SaveInstance->Get<WalkingResultFight>(LevelSaveInstance::DEFAULT_NAME);
	}
protected:
	FString Opponent;
	TArray<TSharedPtr<WalkingResult>> ResultsWin;
};
