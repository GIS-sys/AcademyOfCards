// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyGameInstance.h"

const FString UUMyGameInstance::SAVE_WALKING_PLAYER_STATS = "SAVE_WALKING_PLAYER_STATS";
const FString UUMyGameInstance::SAVE_WALKING_FIGHT_RESULT = "SAVE_WALKING_FIGHT_RESULT";
const FString UUMyGameInstance::SAVE_WALKING_DEALER = "SAVE_WALKING_DEALER";
const FString UUMyGameInstance::SAVE_WALKING_PLAYER_MODEL = "SAVE_WALKING_PLAYER_MODEL";
const FString UUMyGameInstance::SAVE_FIGHTING_PLAYER_STATS = "SAVE_FIGHTING_PLAYER_STATS";
const FString UUMyGameInstance::SAVE_FIGHTING_FIGHT_OUTCOME = "SAVE_FIGHTING_FIGHT_OUTCOME";

void UUMyGameInstance::Init()
{
	Super::Init();

	LoadedWalkingConfigs = MakeShareable(new WalkingConfigs());
	LoadedWalkingConfigs->LoadConfigCards();
	LoadedWalkingConfigs->LoadConfigEvents();

	LoadedFightingConfigs = MakeShareable(new FightingConfigs());
	LoadedFightingConfigs->LoadConfigAbilities(this);
	LoadedFightingConfigs->LoadConfigCards(this);
}