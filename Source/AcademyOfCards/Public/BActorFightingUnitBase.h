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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool IsASpell = false;

	ABActorFightingCellBase* CurrentCell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UFightingUnitParameters* UnitParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainMaterial;

	void SetDiedDuringGame() {
		IsDisplacedOutOfView = true;
	}

	void Move(ABActorFightingField* Field, ABActorFightingCellBase* Cell, int MoveCost = -1);

	void InitUnit();
	void ResetOnTurnEnd(bool TurnEndedIsThisOwner);
	void TakeDamage(int Damage);

	void InitPlayerMy(ABActorFightingField* Field, ABActorFightingCellBase* Cell, const FPlayerStats* Stats);
	void InitPlayerOpponent(ABActorFightingField* Field, FString OpponentName, ABActorFightingCellBase* Cell, FPlayerStats* Stats);

	FString CanMove(ABActorFightingField* Field, ABActorFightingCellBase* Cell);
	FString CanAttack(ABActorFightingField* Field, ABActorFightingUnitBase* Victim);
	bool IsDead(ABActorFightingField* Field);
};
