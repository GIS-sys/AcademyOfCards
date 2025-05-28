// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <deque>
#include <list>

class ABActorFightingField;

static const int MAX_TRIGGERS_PER_EVENT = 100;

struct TriggersDispatcherTrigger {

};

struct TriggersDispatcherEvent {

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

	TriggersDispatcherTrigger* CheckTriggers(int triggered);
	void FlushTriggers(int triggered);

	void AddTriggerAbilitiesFromUnit(ABActorFightingUnitBase* Unit);
};
