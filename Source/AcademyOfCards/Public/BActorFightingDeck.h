// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include <BActorFightingCard.h>
#include <BActorFightingCellBase.h>
#include <UMyGameInstance.h>
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
	UUMyGameInstance* MyGameInstance;
	TArray<FString> CardIDs = { "c001", "c002", "c003", "c004", "c005", "c006", "c007", "c008", "c009", "c010" };
	//TArray<FString> CardIDs = { "c006" };
	// c005 - died
	// c006 - deal damage, random enemy, invocation
	// c007 - on attack, reduce power (current and absolute) by X (min 0)
	// c008 - invocation, random your, +stats
	// c009 - spell, random enemy, damage
	// c010 - spell, random your, +stats, ONATTACHABILITY???

	virtual void BeginPlay() override;

	FString GetRandomCardId() const { return CardIDs[FMath::Rand() % CardIDs.Num()]; };

	UFUNCTION(BlueprintImplementableEvent, Category = "Base")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "Base")
	void DealCards();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool IsControlledByPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector WHERE_IS_HAND;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector MIN_SPACE_BETWEEN_CARDS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector MAX_SPACE_BETWEEN_CARDS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector MAX_HAND_SIZE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingCard> ActorToSpawnCard;

	UFUNCTION(BlueprintCallable, Category = "Base")
	ABActorFightingCard* DrawCard();

	UFUNCTION(BlueprintCallable, Category = "Base")
	ABActorFightingUnitBase* PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell);

	void RearrangeCardsInHand();

	static constexpr int STARTING_HAND = 5;
	static constexpr float DRAWING_TIME = 0.5;
	static constexpr float CARD_PLAY_TIME = 1;
	TArray<ABActorFightingCard*> CardActors;

	ABActorFightingField* BActorFightingField;
};
