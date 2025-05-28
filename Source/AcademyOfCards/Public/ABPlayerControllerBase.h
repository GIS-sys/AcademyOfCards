// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerControllerBase.generated.h"

/**
 * 
 */

UCLASS()
class ACADEMYOFCARDS_API AABPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;
};
