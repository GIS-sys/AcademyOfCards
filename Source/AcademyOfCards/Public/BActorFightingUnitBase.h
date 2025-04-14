// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include <BActorFightingCellBase.h>
#include <FightingUnitParameters.h>
#include <StatStructs.h>
#include "BActorFightingUnitBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingUnitBase : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	float MOVING_TIME = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool IsControlledByPlayer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool IsPlayer = false;

	ABActorFightingCellBase* CurrentCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UFightingUnitParameters* UnitParameters;

	bool Move(ABActorFightingCellBase* Cell);

	void OnSpawn();
	void OnTurnEnd(bool TurnEndedIsThisOwner);
	void OnAttackUnit(ABActorFightingUnitBase* Victim);
	void OnGetAttacked(ABActorFightingUnitBase* Attacker);

	void InitPlayerMy(ABActorFightingCellBase* Cell, const FPlayerStats* Stats);
	void InitPlayerOpponent(FString OpponentName, ABActorFightingCellBase* Cell, FPlayerStats* Stats);
};
