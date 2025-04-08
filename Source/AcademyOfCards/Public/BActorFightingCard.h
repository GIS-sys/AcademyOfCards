// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include <BActorFightingUnitBase.h>
#include <StatStructs.h>
#include "BActorFightingCard.generated.h"

class ABActorFightingCellBase;

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingCard : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool IsControlledByPlayer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool IsBeingPlayed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FMana ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FMana ManaGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString AlignmentKind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	UFightingUnitParameters* UnitParameters;

	ABActorFightingUnitBase* SpawnUnit(ABActorFightingCellBase* Cell, TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit);
};
