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
	ScaleOriginal = GetActorScale3D();
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

	AnimateHighlight();

	if (DeltaTimeMoveOver > 0 && DeltaTimeMoveOverSpent < DeltaTimeMoveOver) {
		DeltaTimeMoveOverSpent += DeltaTime;
		if (DeltaTimeMoveOverSpent > DeltaTimeMoveOver) DeltaTimeMoveOverSpent = DeltaTimeMoveOver;
		float alpha = DeltaTimeMoveOverSpent / DeltaTimeMoveOver;
		alpha = (1 - cosf(alpha * PI)) / 2;
		LocationOriginal = FromLocationMovingTo * (1 - alpha) + NewLocationMovingTo * alpha;
		SetActorLocation(LocationOriginal);
	}

	SetActorLocation(LocationOriginal + LocationDelta + (IsDisplacedOutOfView ? DISPLACEMENT_OUT_OF_VIEW : FVector{0, 0, 0}));
	SetActorRelativeScale3D(ScaleOriginal * ScaleRelative);
}

void ABActorEnhanced::MoveOutOfFrame(float DeltaTime) {
	LocationBeforeOutOfFrame = LocationOriginal;
	MoveOverTimeTo(LocationOriginal, LocationOriginal + LocationDeltaOutOfFrame, DeltaTime);
}

void ABActorEnhanced::MoveIntoFrame(float DeltaTime) {
	MoveOverTimeTo(LocationOriginal, LocationBeforeOutOfFrame, DeltaTime);
}

void ABActorEnhanced::AnimateHighlight()
{
	if (TicksNotHighlighted == -1) return;
	TicksNotHighlighted += 1;
	if (TicksNotHighlighted > 1) {
		OnChangeHighlight();
		OnHighlight();
	} else {
		IsHighlighted = false;
		OnChangeHighlight();
		OnDisHighlight();
	}
}
void ABActorEnhanced::OnChangeHighlight()
{
	
}

void ABActorEnhanced::Highlight()
{
	if (TicksNotHighlighted != -1) TicksNotHighlighted = 0;
	IsHighlighted = true;
}

void ABActorEnhanced::PermanentlyHighlight()
{
	TicksNotHighlighted = -1;
	IsHighlighted = true;
}

void ABActorEnhanced::ResetPermanentHighlight()
{
	TicksNotHighlighted = 2;
}

void ABActorEnhanced::OnHighlight()
{
	LocationDelta = FVector(0.0);
	ScaleRelative = FVector(1.0);
}

void ABActorEnhanced::OnDisHighlight()
{
	LocationDelta = WhenHighlightedLocationDelta;
	ScaleRelative = WhenHighlightedScaleRelative;
}