// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UUMyGameInstance;
class ABActorFightingUnitBase;
class ABActorFightingCellBase;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbility
{
private:
	FightingAbility() {}

public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	~FightingAbility();

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	TSharedPtr<FightingAbility> Build(TSharedPtr<FJsonObject> Arguments);

	void OnMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo);
	void OnSpawn(ABActorFightingUnitBase* OwnerUnit);
	void OnTurnEnd(ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner);
	void OnAttackUnit(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim);
	void OnGetAttacked(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker);
};
