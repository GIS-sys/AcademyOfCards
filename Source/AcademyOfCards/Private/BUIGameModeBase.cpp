// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "BActorWalkingCard.h"


void ABUIGameModeBase::SwitchToFight() {
	// TODO !!!
	UE_LOG(LogTemp, Error, TEXT("Switch To Fight"));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABActorWalkingCard::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors) {
		ABActorWalkingCard* WalkingCard = Cast<ABActorWalkingCard>(Actor);
		WalkingCard->MoveOutOfFrame(MOVE_OUT_OF_FRAME_SECONDS);
	}
}