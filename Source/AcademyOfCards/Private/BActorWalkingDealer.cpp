// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingDealer.h"

// Sets default values
ABActorWalkingDealer::ABActorWalkingDealer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABActorWalkingDealer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABActorWalkingDealer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABActorWalkingDealer::DealCards()
{
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation() + FVector(1.0, 1.0, 1.0), GetActorRotation());
}