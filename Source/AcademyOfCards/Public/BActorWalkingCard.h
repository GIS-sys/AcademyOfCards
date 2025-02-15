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

	void AnimateHighlight();

	int TicksNotHighlighted = 1;
	FVector ScaleRelative;
	FVector LocationDelta;

public:
	struct WallsStruct {
		bool left;
		bool right;
		bool top;
		bool bottom;
	};
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Highlight();

	UFUNCTION(BlueprintCallable)
	void MoveTo();

	UFUNCTION(BlueprintCallable)
	bool GetWallLeft() const { return Walls.left; };
	UFUNCTION(BlueprintCallable)
	bool GetWallRight() const { return Walls.right; };
	UFUNCTION(BlueprintCallable)
	bool GetWallTop() const { return Walls.top; };
	UFUNCTION(BlueprintCallable)
	bool GetWallBottom() const { return Walls.bottom; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainCardMaterial;

	TSharedPtr<WalkingEvent> Event;

	float BoardPositionX;
	float BoardPositionY;

	ABActorWalkingDealer* DealerPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	bool IsDiscovered;

	WallsStruct Walls;
};
