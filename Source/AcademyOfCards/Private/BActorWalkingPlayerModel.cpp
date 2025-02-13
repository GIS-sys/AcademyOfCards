// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingPlayerModel.h"
#include "BActorWalkingdealer.h"

// Sets default values
ABActorWalkingPlayerModel::ABActorWalkingPlayerModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABActorWalkingPlayerModel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABActorWalkingPlayerModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABActorWalkingPlayerModel::Move(FVector LocationTo, int BoardPositionX, int BoardPositionY, ABActorWalkingDealer* DealerPtr)
{
	if (CurrentBoardPositionX != -1 || CurrentBoardPositionY != -1) {
		if (!DealerPtr->CheckAbleToGo(CurrentBoardPositionX, CurrentBoardPositionY, BoardPositionX, BoardPositionY)) return;
		int dx = CurrentBoardPositionX - BoardPositionX; // TODO delete this
		int dy = CurrentBoardPositionY - BoardPositionY;
		if (dx * dx + dy * dy > 1) {
			return;
		}
	}
	CurrentBoardPositionX = BoardPositionX;
	CurrentBoardPositionY = BoardPositionY;
	MoveOverTimeTo(GetActorLocation(), LocationTo, 1.0);
}