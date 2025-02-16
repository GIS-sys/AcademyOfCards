// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class ABActorWalkingCard;
class WalkingEvent;

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
	const TPair<int, int> StartPosition = { 5, -1 };
	const TPair<int, int> FinishPosition = { 2, 4 };
	const int WALLS_TO_DELETE_AMOUNT = 3;
	const int ALLOWED_RANDOM_MISTAKES = 1000;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadConfigEvents();

	bool IsInsideFieldNoEnds(TPair<int, int> point);
	bool IsEnds(TPair<int, int> point);
	bool IsInsideFieldWithEnds(TPair<int, int> point);

	void CreateBoard();
	void DealCard(int ix, int iy);
	void SetTimersForCardDeal();
	void SetPlayerModel(int ix, int iy);
	ABActorWalkingCard* CreateRandomCardFullyBlocked();

	FVector GetCenterCellPosition(int ix, int iy);

	TArray<TSharedPtr<WalkingEvent>> Events;

	const FVector DealingOffset{ 20.0, -440.0, 10.0 };
	const float SpacingBetweenTiles = 0.0;
	const FVector CardSize = FVector({ 50, 50, 1 });

	TArray<TTuple<float, int, int>> DealingCardSpawinRestTime;
	TMap<TPair<int, int>, ABActorWalkingCard*> CardsDealt;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> MaterialArray;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DealCards();
	bool CheckAbleToGo(int CurrentBoardPositionX, int CurrentBoardPositionY, int BoardPositionX, int BoardPositionY);
};
