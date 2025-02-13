// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingCard.h"
#include <WalkingEvent.h>
#include <BActorWalkingPlayerModel.h>
#include <Kismet/GameplayStatics.h>

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

	if (DeltaTimeMoveOver > 0 && DeltaTimeMoveOverSpent < DeltaTimeMoveOver) {
		DeltaTimeMoveOverSpent += DeltaTime;
		LocationOriginal = (FromLocationMovingTo * (DeltaTimeMoveOver - DeltaTimeMoveOverSpent) + NewLocationMovingTo * DeltaTimeMoveOverSpent) / DeltaTimeMoveOver;
	}

	TicksNotHighlighted += 1;
	if (TicksNotHighlighted > 1) {
		LocationDelta = FVector(0.0);
		ScaleRelative = FVector(1.0);
	} else {
		LocationDelta = FVector(0.0, 0.0, 10.0);
		ScaleRelative = FVector(1.5, 1.5, 1.0);
	}

	SetActorLocation(LocationOriginal + LocationDelta);
	SetActorRelativeScale3D(ScaleRelative);
}

void ABActorWalkingCard::MoveOverTimeTo(FVector FromLocation, FVector NewLocation, float DeltaTime)
{
	FromLocationMovingTo = FromLocation;
	NewLocationMovingTo = NewLocation;
	DeltaTimeMoveOver = DeltaTime;
	DeltaTimeMoveOverSpent = 0;
}

void ABActorWalkingCard::Hightlight() {
	TicksNotHighlighted = 0;
}

void ABActorWalkingCard::MoveTo() {
	Event->Fire();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);
	PlayerModel->SetActorLocation(GetActorLocation());
}
