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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainMaterial;

	bool Move(ABActorFightingField* Field, ABActorFightingCellBase* Cell);

	void OnSpawn(ABActorFightingField* Field);
	void OnTurnEnd(ABActorFightingField* Field, bool TurnEndedIsThisOwner);
	void OnAttackUnit(ABActorFightingField* Field, ABActorFightingUnitBase* Victim);
	void OnGetAttacked(ABActorFightingField* Field, ABActorFightingUnitBase* Attacker);

	void InitPlayerMy(ABActorFightingField* Field, ABActorFightingCellBase* Cell, const FPlayerStats* Stats);
	void InitPlayerOpponent(ABActorFightingField* Field, FString OpponentName, ABActorFightingCellBase* Cell, FPlayerStats* Stats);
};
