// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.h"
#include "StatStructs.h"
#include "BActorWalkingPlayerModel.generated.h"
class ABActorWalkingDealer;


UCLASS()
class ACADEMYOFCARDS_API ABActorWalkingPlayerModel : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorWalkingPlayerModel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const int ENERGY_PER_MOVE = 1;
	const float MOVING_TIME = 0.5;

	int CurrentBoardPositionX = -1;
	int CurrentBoardPositionY = -1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Move(FVector LocationTo, int BoardPositionX, int BoardPositionY, ABActorWalkingDealer* DealerPtr);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	FPlayerStats PlayerStats;
};
