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

	StartingPosition = GetActorLocation();
}

// Called every frame
void ABActorWalkingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TotalDeltaTime < 1.0 && TotalDeltaTime + DeltaTime >= 1.0) {
		//FActorSpawnParameters SpawnInfo;
		//SpawnInfo.Owner = this;
		UClass* MyItemBlueprintClass = StaticLoadClass(UObject::StaticClass(), NULL, TEXT("/Content/Actors/BP_WalkingCard.uasset"), NULL, LOAD_None, NULL);
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation() + FVector(1.0, 1.0, 1.0), GetActorRotation());// , SpawnInfo);

		StartingPosition += FVector(0.0, 10.0, 0.0);
	}

	TotalDeltaTime += DeltaTime;
	SetActorLocation(StartingPosition + FVector(0.0, 0.0, 10.0) * (1 + cos(-PI + TotalDeltaTime * 2.0)));
}

