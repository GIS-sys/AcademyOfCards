// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UUMyGameInstance;
class ABActorFightingUnitBase;
class ABActorFightingCellBase;

/**
 * 
 */
enum WHEN {
	ALWAYS = 0,
	INVOCATION = 1,
	ON_ATTACK = 2,
	SPELL_CAST = 3,
	ON_MOVE = 4,
	ON_TURN_END = 5,
	ON_GET_ATTACKED = 6,
};

class ACADEMYOFCARDS_API FightingAbility
{
protected:
	FightingAbility() {}
	virtual void _Build() {}

	WHEN When = ALWAYS;

	virtual void _OnAnything(ABActorFightingUnitBase* OwnerUnit) {}
	virtual void _OnMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo) {}
	virtual void _OnSpawn(ABActorFightingUnitBase* OwnerUnit) {}
	virtual void _OnTurnEnd(ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner) {}
	virtual void _OnAttackUnit(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim) {}
	virtual void _OnGetAttacked(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker) {}

public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	virtual ~FightingAbility() = default;

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	TSharedPtr<FightingAbility> Build(TSharedPtr<FJsonObject> Arguments) const;

	void OnMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo);
	void OnSpawn(ABActorFightingUnitBase* OwnerUnit);
	void OnTurnEnd(ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner);
	void OnAttackUnit(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim);
	void OnGetAttacked(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker);
};
