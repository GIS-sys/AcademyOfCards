// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorWalkingCard.generated.h"
class WalkingEvent;

UCLASS()
class ACADEMYOFCARDS_API ABActorWalkingCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorWalkingCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int TicksNotHighlighted = 1;
	
	FVector LocationDelta;
	FVector ScaleRelative;

	FVector FromLocationMovingTo;
	FVector NewLocationMovingTo;
	float DeltaTimeMoveOver = 0;
	float DeltaTimeMoveOverSpent = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Hightlight();

	void MoveOverTimeTo(FVector FromLocation, FVector NewLocation, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void MoveTo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainCardMaterial;

	FVector LocationOriginal;
	TSharedPtr<WalkingEvent> Event;
};
