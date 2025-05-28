// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <deque>
#include <list>
#include <vector>
#include <any>

class ABActorFightingField;
class ABActorFightingUnitBase;
class FightingAbility;

static const int MAX_TRIGGERS_PER_EVENT = 100;


struct TriggersDispatcherTrigger;
struct TriggersDispatcherEvent {
	enum EnumAbility {
		DrawCard = 0,
		PassTurn,
		GetManaLight,
		GetManaDark,
		GetManaFire,
		GetManaIce,
	};

	enum EnumEvent {
		MOVE = 0,
		ATTACK,
		PLAYCARD,
		DRAWCARD,
		DrawCardOnTurnStart,
	};

	int type = -1; // 0 = trigger, 1 = ability, 2 = event
	TriggersDispatcherTrigger* trigger;
	EnumAbility ability;
	std::vector<std::any> ability_args;
	EnumEvent event;
	std::vector<std::any> event_args;

	static TriggersDispatcherEvent MakeTriggerTriggered(TriggersDispatcherTrigger*);
	static TriggersDispatcherEvent MakeAbility(EnumAbility, std::vector<std::any> args = {});
	static TriggersDispatcherEvent MakeEvent(EnumEvent, std::vector<std::any> args = {});
};
struct TriggersDispatcherTrigger {
	ABActorFightingUnitBase* unit;
	TSharedPtr<FightingAbility> ability;

	bool Check(ABActorFightingField* Field, TriggersDispatcherEvent Event);
	void Exec(ABActorFightingField* Field, TriggersDispatcherEvent Event);
	void Flush(ABActorFightingField* Field, TriggersDispatcherEvent Event);

	static TriggersDispatcherTrigger MakeAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability);
};


class ACADEMYOFCARDS_API FightingTriggersDispatcher
{
	ABActorFightingField* Field = nullptr;
	int triggered = 0;

public:
	std::list<TriggersDispatcherTrigger> all_triggers;
	std::deque<TriggersDispatcherEvent> all_events;
	std::deque<TriggersDispatcherEvent> history_events;

	FightingTriggersDispatcher();
	~FightingTriggersDispatcher();

	void Init(ABActorFightingField* NewField);
	void Tick(float DeltaTime);

	TriggersDispatcherTrigger* CheckTriggers();
	void FlushTriggers();

	void AddTriggerAbilitiesFromUnit(ABActorFightingUnitBase* Unit);
	void AddTriggerAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability);

	void AddEvent(TriggersDispatcherEvent Event);
};
