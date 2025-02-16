// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingPlayerModel.h"
#include <BActorWalkingCard.h>
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

bool ABActorWalkingPlayerModel::Move(FVector LocationTo, int BoardPositionX, int BoardPositionY, ABActorWalkingDealer* DealerPtr)
{
	// check ability to move
	if (CurrentBoardPositionX != -1 || CurrentBoardPositionY != -1) {
		if (!DealerPtr->CheckAbleToGo(CurrentBoardPositionX, CurrentBoardPositionY, BoardPositionX, BoardPositionY)) return false;
	}
	// spend energy
	if (!DealerPtr->CardsDealt.IsEmpty() && !DealerPtr->CardsDealt[TPair<int, int>(BoardPositionX, BoardPositionY)]->IsDiscovered) {
		PlayerStats.Energy -= ENERGY_PER_MOVE;
	}
	// rotate model
	float RotationDeg = 0;
	if (CurrentBoardPositionX - BoardPositionX == 1) RotationDeg = 90;
	if (CurrentBoardPositionX - BoardPositionX == -1) RotationDeg = -90;
	if (CurrentBoardPositionY - BoardPositionY == 1) RotationDeg = 180;
	if (CurrentBoardPositionY - BoardPositionY == -1) RotationDeg = 0;
	SetActorRotation(FRotator(0, RotationDeg, 0));
	// move
	CurrentBoardPositionX = BoardPositionX;
	CurrentBoardPositionY = BoardPositionY;
	MoveOverTimeTo(GetActorLocation(), LocationTo, MOVING_TIME);
	return true;
}