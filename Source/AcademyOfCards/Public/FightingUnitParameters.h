// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingAbility.h"
#include "StatStructs.h"
#include "FightingUnitParameters.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ACADEMYOFCARDS_API UFightingUnitParameters : public UObject
{
	GENERATED_BODY()
public:
	void FromFUnitParameters(const FUnitParameters& other) {
		IsUnit = other.IsUnit;
		Movement = other.Movement;
		Health = other.Health;
		Power = other.Power;
		Attacks = other.Attacks;
		Range = other.Range;
		Abilities = other.Abilities;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool IsUnit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Movement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Attacks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Range = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString Name;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentMovement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentHealth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentPower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentAttacks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentRange = 0;

	TArray<TSharedPtr<FightingAbility>> Abilities;

	void ResetCurrent(bool on_spawn = false);
};
