// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.h"
#include "BActorWalkingPlayerModel.generated.h"
class ABActorWalkingDealer;


//USTRUCT(BlueprintType)
//struct FPlayerStats
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadWrite)
//	int32 X;
//
//	UPROPERTY(BlueprintReadWrite)
//	float Y;
//};


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

	int CurrentBoardPositionX = -1;
	int CurrentBoardPositionY = -1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Move(FVector LocationTo, int BoardPositionX, int BoardPositionY, ABActorWalkingDealer* DealerPtr);
};
