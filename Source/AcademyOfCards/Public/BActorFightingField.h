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

UCLASS()
class ACADEMYOFCARDS_API ABActorFightingField : public ABActorEnhanced
{
	GENERATED_BODY()

public:
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

	void InitPlayers();

	void InitDecks();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckMy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckOpponent;

	TArray<ABActorFightingUnitBase*> ArrayUnits;
	void InitUnits();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void MoveUnit(ABActorFightingUnitBase* Unit);

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
};
