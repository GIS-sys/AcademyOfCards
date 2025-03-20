// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Transition")
	void Disappear(float Seconds);
};
