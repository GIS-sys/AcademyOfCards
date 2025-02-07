// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingCamera.h"

// Sets default values
ABActorWalkingCamera::ABActorWalkingCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	RootComponent = CameraComponent;

	//CurrentAngle = 0.0f; // Initialize the angle
}

// Called when the game starts or when spawned
void ABActorWalkingCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABActorWalkingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator LookAtRotation = (GetActorLocation() - FVector(1.0, 1.0, 1.0)).Rotation();
	CameraComponent->SetWorldRotation(LookAtRotation);
}

