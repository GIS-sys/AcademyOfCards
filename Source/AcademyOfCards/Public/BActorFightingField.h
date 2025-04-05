// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BActorEnhanced.h>
#include <BActorFightingUnitBase.h>
#include <BActorFightingCellBase.h>
#include <BActorFightingDeck.h>
#include "BActorFightingField.generated.h"

UCLASS()
class ACADEMYOFCARDS_API ABActorFightingField : public ABActorEnhanced
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<ABActorEnhanced>> CardsSubclasses;

	/*ABActorFightingDeck* DeckMy;
	ABActorFightingDeck* DeckOpponent;*/
	void InitDecks();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	//TSubclassOf<ABActorFightingDeck> ActorToSpawnDecks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckMy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	ABActorFightingDeck* DeckOpponent;

	TArray<ABActorFightingUnitBase*> ArrayUnits;
	void InitUnits();

	const int RADIUS = 5;
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
};
