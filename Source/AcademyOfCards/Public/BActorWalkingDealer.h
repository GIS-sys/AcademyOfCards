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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadConfigEvents();

	void DealCard(int ix, int iy);

	TArray<TSharedPtr<WalkingEvent>> Events;

	const FVector DealingOffset{ 50.0, -300.0, 10.0 };
	const float SpacingBetweenTiles = 0.0;

	TArray<TTuple<float, int, int>> DealingCardSpawinRestTime;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	TArray<UMaterialInterface*> MaterialArray;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DealCards();
};
