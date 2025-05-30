// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fighting/FightingTriggersDispatcher.h"
#include <set>

class UUMyGameInstance;
class ABActorFightingUnitBase;
class ABActorFightingCellBase;
class ABActorFightingField;

/**
 * 
 */

enum WHEN : int {
	NONE = 0,
	INVOCATION,
	//ON_ATTACK, // TODO IMPORTANT
	//SPELL_CAST,
	//ON_MOVE,
	//ON_TURN_END,
	//ON_GET_ATTACKED,
	//ALWAYS,
};

// TODO IMPORTANT conditions in fighting abilities
// "Allied_familiar_died_this_turn": { "greater": 0 }
// "Previous succed": 1
// "attack_opponent_destroyed": 1

class ACADEMYOFCARDS_API FightingAbility
{
protected:
	FightingAbility() {}
	virtual void _Build() {}

	std::set<WHEN> When;

public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	virtual ~FightingAbility() = default;

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	TSharedPtr<FightingAbility> Build(TSharedPtr<FJsonObject> Arguments) const;

	virtual bool CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return false; }
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; }
	virtual void FlushEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; }

	virtual void CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }
	virtual void CanAttack(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingUnitBase* Victim, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }
	virtual void IsDead(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }
};
