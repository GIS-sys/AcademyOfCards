// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSaveInstance.h"

class WalkingDeck;
class ABActorWalkingCard;

#include "BActorWalkingDealer.generated.h"


UCLASS()
class ACADEMYOFCARDS_API ABActorWalkingDealer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorWalkingDealer();

protected:
	const int FieldWidth = 8;
	const int FieldHeight = 4;
	const TPair<int, int> StartPosition = { 2, 4 };
	const TPair<int, int> FinishPosition = { 5, -1 };
	const int WALLS_TO_DELETE_AMOUNT = 3;
	const int ALLOWED_RANDOM_MISTAKES = 1000;
	const float TIME_CARD_DEALING_MOVEMENT = 0.2;
	const float TIME_BETWEEN_CARD_DEAL = 0.08;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsInsideFieldNoEnds(TPair<int, int> point);
	bool IsEnds(TPair<int, int> point);
	bool IsInsideFieldWithEnds(TPair<int, int> point);

	void CreateBoard();
	void DealCard(int ix, int iy);
	void SetTimersForCardDeal();
	void SetPlayerModel(int ix, int iy);
	ABActorWalkingCard* CreateRandomCardFullyBlocked();

	FVector GetCenterCellPosition(int ix, int iy);

	const float SpacingBetweenTiles = 0.0;

	TArray<TTuple<float, int, int>> DealingCardSpawnRestTime;

public:
	TSharedPtr<WalkingDeck> Deck;

	TMap<TPair<int, int>, ABActorWalkingCard*> CardsDealt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	FString DEBUG_CARD_ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector DealingOffset { -660.0, -660.0, -20.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector CardSize = FVector({ 100, 100, 1 });

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorWhereToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> MaterialArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<FString> MaterialIDsArray;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DealCards();
	bool CheckAbleToGo(int CurrentBoardPositionX, int CurrentBoardPositionY, int BoardPositionX, int BoardPositionY);

	LevelSaveInstance Save();
	void Load(LevelSaveInstance* SaveInstance);
};
