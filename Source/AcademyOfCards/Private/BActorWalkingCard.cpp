// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingCard.h"
#include <WalkingEvent.h>
#include <BActorWalkingPlayerModel.h>
#include <Kismet/GameplayStatics.h>
#include "WalkingDeck.h"
#include "WalkingCardConfig.h"

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
}

void ABActorWalkingCard::AnimateHighlight()
{
	TicksNotHighlighted += 1;
	if (TicksNotHighlighted > 1) {
		LocationDelta = FVector(0.0);
		ScaleRelative = FVector(1.0);
	}
	else {
		LocationDelta = FVector(0.0, 0.0, 1.0);
		ScaleRelative = FVector(1.1, 1.1, 1.0);
	}
}

// Called every frame
void ABActorWalkingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimateHighlight();

	SetActorLocation(LocationOriginal + LocationDelta);
	SetActorRelativeScale3D(ScaleRelative);
}

void ABActorWalkingCard::Highlight() {
	TicksNotHighlighted = 0;
}

void ABActorWalkingCard::MoveTo() {
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);
	if (PlayerModel->Move(GetActorLocation(), BoardPositionX, BoardPositionY, DealerPtr)) {
		if (!IsDiscovered) {
			IsDiscovered = true;
			WalkingDeck->GetEventByID(CardConfig->GetEventFired(PlayerModel->PlayerStats))->Fire(DealerPtr);
		}
	}
}
