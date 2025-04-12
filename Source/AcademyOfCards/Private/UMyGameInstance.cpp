// Fill out your copyright notice in the Description page of Project Settings.


#include "UMyGameInstance.h"
#include "WalkingDeck.h"

const FString UUMyGameInstance::SAVE_WALKING_PLAYER_STATS = "SAVE_WALKING_PLAYER_STATS";
const FString UUMyGameInstance::SAVE_WALKING_FIGHT_RESULT = "SAVE_WALKING_FIGHT_RESULT";
const FString UUMyGameInstance::SAVE_WALKING_DEALER = "SAVE_WALKING_DEALER";
const FString UUMyGameInstance::SAVE_WALKING_PLAYER_MODEL = "SAVE_WALKING_PLAYER_MODEL";
const FString UUMyGameInstance::SAVE_FIGHTING_PLAYER_STATS = "SAVE_FIGHTING_PLAYER_STATS";
const FString UUMyGameInstance::SAVE_FIGHTING_FIGHT_OUTCOME = "SAVE_FIGHTING_FIGHT_OUTCOME";

void UUMyGameInstance::Init()
{
	Super::Init();

	Deck = MakeShareable(new WalkingDeck());
	Deck->LoadConfigCards();
	Deck->LoadConfigEvents();

	// TODO load unit/abilities config
}