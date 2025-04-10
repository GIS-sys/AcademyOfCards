// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BActorEnhanced.h>
#include <BActorFightingUnitBase.h>
#include <BActorFightingCellBase.h>
#include <BActorFightingDeck.h>
#include "StatStructs.h"
#include "BActorFightingField.generated.h"

class ABActorFightingField;

class AI {
	bool NeedLoop = false;
	int WasThinkingFor = 0;
	TTuple<int, int, int> MovingPlayerCoordinates;
	TTuple<int, int, int> PlayCardCoordinates;
	ABActorFightingCard* CardToPlay;

	void InitNextTurn(ABActorFightingField* FightingField);
	
	void Think(ABActorFightingField* FightingField);
	void Act(ABActorFightingField* FightingField);
	bool HasFinishedTurn(ABActorFightingField* FightingField);

	void StartThinkingLoop(ABActorFightingField* FightingField);

public:
	void YourTurn(ABActorFightingField* FightingField);
};

UCLASS()
class ACADEMYOFCARDS_API ABActorFightingField : public ABActorEnhanced
{
	GENERATED_BODY()

public:
	ABActorFightingUnitBase* GetCurrentPlayerUnit() const {
		if (IsPlayerTurn) return PlayerUnitMy;
		return PlayerUnitOpponent;
	}
	FPlayerStats* GetPlayerStats(bool IsPlayerMe);
	FPlayerStats* GetCurrentPlayerStats() {
		return GetPlayerStats(IsPlayerTurn);
	}
	FPlayerMana* GetCurrentPlayerMana() {
		if (IsPlayerTurn) return &PlayerMana;
		return &OpponentMana;
	}
	ABActorFightingDeck* GetCurrentPlayerDeck() {
		if (IsPlayerTurn) return DeckMy;
		return DeckOpponent;
	}
	bool IsOccupied(const TTuple<int, int, int>& Coordinates) const {
		for (ABActorFightingUnitBase* Unit : ArrayUnits) {
			if (Unit->CurrentCell->GetCoordinates() == Coordinates) return true;
		}
		return false;
	}
	bool IsOccupied(ABActorFightingCellBase* Cell) const {
		for (ABActorFightingUnitBase* Unit : ArrayUnits) {
			if (Unit->CurrentCell == Cell) return true;
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	bool IsFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	bool IsPlayerWinner = false;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	FPlayerMana PlayerMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	FPlayerMana OpponentMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	FPlayerStats OpponentStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	bool IsPlayerTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<ABActorEnhanced>> CardsSubclasses;

	static constexpr int PLAYER_START_MY_X = 2;
	static constexpr int PLAYER_START_MY_Y = 0;
	static constexpr int PLAYER_START_MY_Z = 1;
	static constexpr int PLAYER_START_OPPONENT_X = 0;
	static constexpr int PLAYER_START_OPPONENT_Y = 2;
	static constexpr int PLAYER_START_OPPONENT_Z = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	ABActorFightingUnitBase* PlayerUnitMy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	ABActorFightingUnitBase* PlayerUnitOpponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players")
	FString OpponentName;
	void InitPlayers();

	void InitLoadFromWalking();

	void InitDecks();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckMy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckOpponent;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell);

	TArray<ABActorFightingUnitBase*> ArrayUnits;
	void InitUnits();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool MoveUnit(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell);
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	bool AttackUnit(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim);

	const int RADIUS = 4;
	TArray<TArray<TArray<ABActorFightingCellBase*>>> ArrayCells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingCellBase> ActorToSpawnCells;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* SceneComponentCells;
	void InitCells();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float COLUMN_SHOW_HEIGHT = 20.0;

	UFUNCTION(BlueprintCallable, Category = "Base")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString AbilityDrawCard();
	FString AbilityGetMana(int& Mana);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString AbilityGetManaLight();
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString AbilityGetManaDark();
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString AbilityGetManaFire();
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString AbilityGetManaIce();
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FString PassTurn();

	AI AIOpponent;
};
