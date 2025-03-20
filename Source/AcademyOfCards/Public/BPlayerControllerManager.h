// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPlayerControllerManager.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABPlayerControllerManager : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	TSubclassOf<APlayerController> PlayerControllerWalkingStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controllers")
	TSubclassOf<APlayerController> PlayerControllerFightStage;
};
