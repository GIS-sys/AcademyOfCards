// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BActorEnhanced.h>
#include <BActorFightingUnitBase.h>
#include <BActorFightingCellBase.h>
#include <BActorFightingDeck.h>
#include "StatStructs.h"
#include "Fighting/FightingUIManager.h"
#include "Fighting/FightingTriggersDispatcher.h"
#include <map>
#include "BActorFightingField.generated.h"


class ABActorFightingField;

class AI {
	bool NeedLoop = false;
	int WasThinkingFor = 0;
	TTuple<int, int, int> MovingPlayerCoordinates;
	TTuple<int, int, int> PlayCardCoordinates;
	ABActorFightingCard* CardToPlay;
	TTuple<int, int, int> MoveUnitCoordinates = { -1, -1, -1 };
	ABActorFightingUnitBase* MoveUnit = nullptr;
	ABActorFightingUnitBase* AttackUnitAttacker = nullptr;
	ABActorFightingUnitBase* AttackUnitVictim = nullptr;

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

	ABActorFightingUnitBase* GetRandomUnitNear(ABActorFightingCellBase* Cell, int Range = 1) {
		TArray<ABActorFightingUnitBase*> NearUnits;
		for (ABActorFightingUnitBase* Neighbour : ArrayUnits) {
			if (ABActorFightingCellBase::Distance(Neighbour->CurrentCell, Cell) <= Range) NearUnits.Add(Neighbour);
		}
		if (NearUnits.IsEmpty()) return nullptr;
		return NearUnits[FMath::Rand() % NearUnits.Num()];
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> MaterialCardArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> MaterialUnitArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FString> MaterialCardIDsArray;

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

	TArray<ABActorFightingUnitBase*> ArrayUnits;
	void InitUnits();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit;

	const int RADIUS = 4;
	TArray<TArray<TArray<ABActorFightingCellBase*>>> ArrayCells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingCellBase> ActorToSpawnCells;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* SceneComponentCells;
	void InitCells();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	float COLUMN_SHOW_HEIGHT = 160.0;

	UFUNCTION(BlueprintCallable, Category = "Base")
	void Init();

	FString AbilityDrawCardWithEvent();
	FString AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility ManaType);
	FString AbilityGetManaLightWithEvent();
	FString AbilityGetManaDarkWithEvent();
	FString AbilityGetManaFireWithEvent();
	FString AbilityGetManaIceWithEvent();
	FString PassTurnWithEvent(bool DoEvent = true);

	FString MoveUnitWithEvent(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell);
	FString DealDamageWithEvent(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim, int Damage);
	FString AttackUnitWithEvent(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim);
	FString PlayCardWithEvent(ABActorFightingCard* Card, ABActorFightingCellBase* Cell);

	FString DeleteUnit(ABActorFightingUnitBase* Unit);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnCell(ABActorFightingCellBase* target);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnUnit(ABActorFightingUnitBase* target);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnCard(ABActorFightingCard* target);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnPassTurn();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FString ClickedOnOutside();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnCell();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnUnit();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnAbility();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnCard();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnPassTurn();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool CanClickOnOutside();

	FightingTriggersDispatcher TriggersDispatcher;
	FightingUIManager UIManager;

	AI AIOpponent;
};
