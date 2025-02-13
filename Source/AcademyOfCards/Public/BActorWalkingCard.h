// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.h"
#include "BActorWalkingCard.generated.h"
class WalkingEvent;
class ABActorWalkingDealer;

UCLASS()
class ACADEMYOFCARDS_API ABActorWalkingCard : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorWalkingCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int TicksNotHighlighted = 1;
	FVector ScaleRelative;
	FVector LocationDelta;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Hightlight();

	UFUNCTION(BlueprintCallable)
	void MoveTo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainCardMaterial;

	TSharedPtr<WalkingEvent> Event;

	float BoardPositionX;
	float BoardPositionY;
	ABActorWalkingDealer* DealerPtr;
};
