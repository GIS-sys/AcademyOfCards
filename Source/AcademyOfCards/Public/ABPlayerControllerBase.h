// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerControllerBase.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class ACADEMYOFCARDS_API UControllerFightState : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Helper")
    int state = 0;

    UFUNCTION(BlueprintCallable, Category = "Helper")
    bool CanSelectCell()
    {
        return state == 1 || state == 2;
    }

    UFUNCTION(BlueprintCallable, Category = "Helper")
    bool CanSelectCard()
    {
        return state == 0;
    }

    UFUNCTION(BlueprintCallable, Category = "Helper")
    bool CanSelectUnit()
    {
        return state == 0;
    }
};

UCLASS()
class ACADEMYOFCARDS_API AABPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    UControllerFightState* ControllerFightState;
};
