// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorEnhanced.h"

// Sets default values
ABActorEnhanced::ABActorEnhanced()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABActorEnhanced::BeginPlay()
{
	Super::BeginPlay();
	
	LocationOriginal = GetActorLocation();
}

void ABActorEnhanced::MoveOverTimeTo(FVector FromLocation, FVector NewLocation, float DeltaTime)
{
	FromLocationMovingTo = FromLocation;
	NewLocationMovingTo = NewLocation;
	DeltaTimeMoveOver = DeltaTime;
	DeltaTimeMoveOverSpent = 0;
}

// Called every frame
void ABActorEnhanced::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DeltaTimeMoveOver > 0 && DeltaTimeMoveOverSpent < DeltaTimeMoveOver) {
		DeltaTimeMoveOverSpent += DeltaTime;
		if (DeltaTimeMoveOverSpent > DeltaTimeMoveOver) DeltaTimeMoveOverSpent = DeltaTimeMoveOver;
		float alpha = DeltaTimeMoveOverSpent / DeltaTimeMoveOver;
		alpha = (1 - cosf(alpha * PI)) / 2;
		LocationOriginal = FromLocationMovingTo * (1 - alpha) + NewLocationMovingTo * alpha;
		SetActorLocation(LocationOriginal);
	}
}

