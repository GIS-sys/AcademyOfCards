// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingCard.h"

// Sets default values
ABActorWalkingCard::ABActorWalkingCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABActorWalkingCard::BeginPlay()
{
	Super::BeginPlay();

	LocationOriginal = GetActorLocation();
}

// Called every frame
void ABActorWalkingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TicksNotHighlighted += 1;
	if (TicksNotHighlighted > 1) {
		LocationDelta = FVector(0.0);
		ScaleRelative = FVector(1.0);
	} else {
		LocationDelta = FVector(10.0, 10.0, 10.0);
		ScaleRelative = FVector(2.0, 2.0, 1.0);
	}

	SetActorLocation(LocationOriginal + LocationDelta);
	SetActorRelativeScale3D(ScaleRelative);
}

void ABActorWalkingCard::Hightlight() {
	TicksNotHighlighted = 0;
}
