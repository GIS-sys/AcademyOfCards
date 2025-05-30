// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <deque>
#include <list>
#include <vector>
#include <any>
#include <functional>
#include <map>
#include "MyEnumsCollection.h"

class ABActorFightingField;
class ABActorFightingUnitBase;
class FightingAbility;

static const int MAX_TRIGGERS_PER_EVENT = 1000;

struct TriggersDispatcherTrigger;
struct TriggersDispatcherEvent {
	enum Type : int {
		NONE = -1,
		TRIGGER,
		ABILITY,
		EVENT,
	};
	Type type = Type::NONE;
	TriggersDispatcherTrigger* trigger;
	TriggersDispatcherEvent_EnumAbility ability;
	std::map<FString, std::any> ability_args;
	std::map<FString, std::any> old_ability_args;
	TriggersDispatcherEvent_EnumEvent event;
	std::map<FString, std::any> event_args;
	std::map<FString, std::any> old_event_args;

	std::function<FString(std::map<FString, std::any>&)> CallbackFoo;

	FString ToDebugString() const;
	void Callback();

	static TriggersDispatcherEvent MakeTriggerTriggered(TriggersDispatcherTrigger*);
	static TriggersDispatcherEvent MakeAbility(TriggersDispatcherEvent_EnumAbility enum_ability, std::map<FString, std::any> args = {}, std::function<FString(std::map<FString, std::any>&)> callback = [](std::map<FString, std::any>) { return "";  });
	static TriggersDispatcherEvent MakeEvent(TriggersDispatcherEvent_EnumEvent enum_event, std::map<FString, std::any> args = {}, std::function<FString(std::map<FString, std::any>&)> callback = [](std::map<FString, std::any>) { return "";  });
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
	void DeleteTriggerAbility(TSharedPtr<FightingAbility> Ability);
};
