// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fighting/FightingTriggersDispatcher.h"

class UUMyGameInstance;
class ABActorFightingUnitBase;
class ABActorFightingCellBase;
class ABActorFightingField;

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

public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	virtual ~FightingAbility() = default;

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	TSharedPtr<FightingAbility> Build(TSharedPtr<FJsonObject> Arguments) const;

	virtual bool CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return false; } // TODO IMPORTANT
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; } // TODO IMPORTANT
	virtual void FlushEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; } // TODO IMPORTANT

	virtual bool CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* Cell, bool can_move, ABActorFightingField* Field) { return can_move; } // TODO IMPORTANT
	virtual bool CanAttack(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim, bool can_attack, ABActorFightingField* Field) { return can_attack; } // TODO IMPORTANT
	virtual bool IsDead(ABActorFightingUnitBase* OwnerUnit, bool is_dead, ABActorFightingField* Field) { return is_dead; } // TODO IMPORTANT
};
