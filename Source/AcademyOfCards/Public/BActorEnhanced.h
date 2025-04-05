// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.generated.h"

UCLASS()
class ACADEMYOFCARDS_API ABActorEnhanced : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorEnhanced();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector FromLocationMovingTo;
	FVector NewLocationMovingTo;
	float DeltaTimeMoveOver = 0;
	float DeltaTimeMoveOverSpent = 0;

	FVector LocationBeforeOutOfFrame;
	const FVector LocationDeltaOutOfFrame{ 0.0, 0.0, 1000.0 };

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveOverTimeTo(FVector FromLocation, FVector NewLocation, float DeltaTime);

	void MoveOutOfFrame(float DeltaTime);
	void MoveIntoFrame(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector LocationOriginal{ 0, 0, 0 };
};
