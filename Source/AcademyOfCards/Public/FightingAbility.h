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

enum WHEN : int { // TODO IMPORTANT
	NONE = 0,
	INVOCATION,
	ON_ATTACK,
	ON_DEATH,
	//SPELL_CAST,
	//ON_MOVE,
	//ON_TURN_END,
	//ON_GET_ATTACKED,
	//ALWAYS,
};

struct FightingAbilityCondition {
	FString Type;
	TSharedPtr<FJsonValue> Data;

	FightingAbilityCondition() {}
	FightingAbilityCondition(FString Type, TSharedPtr<FJsonValue> Data) : Type(Type), Data(Data) {}

	bool Check(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) const;
};

struct FightingAbilityTarget {
	TSharedPtr<FJsonObject> Data;

	void With(
		ABActorFightingField* Field,
		TriggersDispatcherEvent& Event,
		ABActorFightingUnitBase* OwnerUnit,
		std::function<void(const std::map<FString, std::any>&, ABActorFightingField*, TriggersDispatcherEvent&, ABActorFightingUnitBase*)> Callback
	) const;
};

class ACADEMYOFCARDS_API FightingAbility
{
protected:
	FightingAbility() {}
	virtual void _Build() {}

public:
	FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance);
	virtual ~FightingAbility() = default;

	static TSharedPtr<FightingAbility> FactoryBuildSpellDeath();

	FString ID;
    FString Type;
    FString Description;
    TSharedPtr<FJsonObject> Arguments;
	TSharedPtr<FJsonObject> AdditionalArguments;

	std::set<WHEN> When;
	std::map<FString, FightingAbilityCondition> Conditions;
	FightingAbilityTarget Target;

	TSharedPtr<FightingAbility> Build(TSharedPtr<FJsonObject> Arguments) const;

	virtual bool CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return ChecWhens(Event, OwnerUnit) && CheckConditions(Field, Event, OwnerUnit); }
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; }
	virtual void FlushEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { return; }

	virtual void CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }
	virtual void CanAttack(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingUnitBase* Victim, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }
	virtual void IsDead(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, std::map<FString, std::any>& args, ABActorFightingField* Field) { return; }



	bool CheckIsWhenApplicable(const WHEN& when, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { // TODO IMPORTANT
		if (when == WHEN::INVOCATION) {
			// Only react to event PLAYED
			if (Event.type != TriggersDispatcherEvent::Type::EVENT || Event.event != TriggersDispatcherEvent_EnumEvent::PLAYED_CARD) return false;
			// Only react to moving yourself
			ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(Event.event_args["created_unit"]);
			if (TargetUnit != OwnerUnit) return false;
			return true;
		} else if (when == WHEN::ON_ATTACK) {
			// Only react to event ATTACKED
			if (Event.type != TriggersDispatcherEvent::Type::EVENT || Event.event != TriggersDispatcherEvent_EnumEvent::ATTACKED) return false;
			// Only react to attacking by yourself
			ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(Event.event_args["attacker"]);
			if (TargetUnit != OwnerUnit) return false;
			return true;
		} else if (when == WHEN::ON_DEATH) {
			// Only react to event UNIT_DIED
			if (Event.type != TriggersDispatcherEvent::Type::EVENT || Event.event != TriggersDispatcherEvent_EnumEvent::UNIT_DIED) return false;
			// Only react to death by yourself
			ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(Event.event_args["unit"]);
			if (TargetUnit != OwnerUnit) return false;
			return true;
		} else {
			UE_LOG(LogTemp, Error, TEXT("ERROR UNKNOWN WHEN TYPE %d (CheckIsWhenApplicable)"), (int)when);
			return false;
		}
	}

	bool ChecWhens(TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { // TODO IMPORTANT
		bool applicable = false;
		for (const auto& when : When) {
			applicable = CheckIsWhenApplicable(when, Event, OwnerUnit);
			if (applicable) break;
		}
		return applicable;
	}

	bool CheckConditions(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { // TODO IMPORTANT
		bool applicable = true;
		for (const auto& condition : Conditions) {
			applicable = condition.second.Check(Field, Event, OwnerUnit);
			if (!applicable) break;
		}
		return applicable;
	}
};
