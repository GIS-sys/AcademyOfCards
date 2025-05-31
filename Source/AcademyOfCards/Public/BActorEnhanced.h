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

	// Movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveOverTimeTo(FVector FromLocation, FVector NewLocation, float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsMovingOverTime() const {
		return DeltaTimeMoveOver > 0 && DeltaTimeMoveOverSpent < DeltaTimeMoveOver;
	}

	void MoveOutOfFrame(float DeltaTime);
	void MoveIntoFrame(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector LocationOriginal{ 0, 0, 0 };
	FVector ScaleOriginal{ 1, 1, 1 };

	// Highlighting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool IsHighlighted = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool IsDisplacedOutOfView = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Highlight")
	FVector DISPLACEMENT_OUT_OF_VIEW{ 1000, 1000, 1000 };

	int TicksNotHighlighted = 1;

	void AnimateHighlight();

	virtual void OnHighlight();
	virtual void OnDisHighlight();
	virtual void OnChangeHighlight();

	FVector ScaleRelative{ 1, 1, 1 };
	FVector LocationDelta{ 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Highlight")
	FVector WhenHighlightedScaleRelative{ 1.1, 1.1, 1.0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Highlight")
	FVector WhenHighlightedLocationDelta{ 0.0, 0.0, 1.0 };

	UFUNCTION(BlueprintCallable, Category = "Highlight")
	void Highlight();
	UFUNCTION(BlueprintCallable, Category = "Highlight")
	void PermanentlyHighlight();
	UFUNCTION(BlueprintCallable, Category = "Highlight")
	void ResetPermanentHighlight();
};
