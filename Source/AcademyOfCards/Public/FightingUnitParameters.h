// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingUnitParameters.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ACADEMYOFCARDS_API UFightingUnitParameters : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Movement = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentMovement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentHealth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentPower = 0;

	void ResetCurrent();
};
