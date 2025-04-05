// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include <BActorFightingCard.h>
#include <BActorFightingCellBase.h>
#include "BActorFightingDeck.generated.h"

class ABActorFightingField;

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingDeck : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Base")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "Base")
	void DealCards();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector WHERE_IS_HAND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector SPACE_BETWEEN_CARDS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingCard> ActorToSpawnCard;

	UFUNCTION(BlueprintCallable, Category = "Base")
	void DrawCard(int index);

	UFUNCTION(BlueprintCallable, Category = "Base")
	void PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell);

	void RearrangeCardsInHand();

	static constexpr int STARTING_HAND = 5;
	static constexpr float DRAWING_TIME = 0.5;
	static constexpr float CARD_PLAY_TIME = 1;
	TArray<ABActorFightingCard*> CardActors;

	ABActorFightingField* BActorFightingField;
};
