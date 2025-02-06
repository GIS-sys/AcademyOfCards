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
	for (int ix = 0; ix < 4; ++ix) {
		for (int iy = 0; iy < 10; ++iy) {
			AActor* actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation());
			FVector size = actor->GetComponentsBoundingBox(false, true).GetSize();
			float x = ix * size.X * 1.5;
			float y = iy * size.Y * 1.5;
			actor->SetActorLocation(GetActorLocation() + FVector(x, y, 0.0));
		}
	}
}