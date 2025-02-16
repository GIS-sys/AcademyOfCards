// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.h"
#include "BActorWalkingPlayerModel.generated.h"
class ABActorWalkingDealer;


USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Energy = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Gold = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentLight = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentFire = 0.0;
};


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
