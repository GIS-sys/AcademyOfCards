// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighting/FightingTriggersDispatcher.h"
#include "BActorFightingField.h"

FightingTriggersDispatcher::FightingTriggersDispatcher()
{
}

FightingTriggersDispatcher::~FightingTriggersDispatcher()
{
}

void FightingTriggersDispatcher::Init(ABActorFightingField* NewField)
{
    Field = NewField;
    triggered = 0;
}

void FightingTriggersDispatcher::Tick(float DeltaTime)
{
    if (!Field) return;

    if (Field->UIManager.IsWaitingPlayerResponse() || !Field->IsPlayerTurn || all_events.empty()) return;

    TriggersDispatcherTrigger* trigger = CheckTriggers(triggered);
    if (trigger && triggered < MAX_TRIGGERS_PER_EVENT) {
        ++triggered;
        all_events.push_back(TriggersDispatcherEvent.MakeTriggerTriggered(trigger));
        trigger->Exec(Field, all_events[0]);
        return;
    }

    FlushTriggers(triggered);
    triggered = 0;
    all_events.pop_front();
    UIManager.LetActionsRegular();
}

Trigger* FightingTriggersDispatcher::CheckTriggers(int triggered)
{
    int i = 0;
    for (auto& trigger : all_triggers) {
        if (trigger.Check(Field, all_events[0])) return &trigger;
        ++i;
    }
    return nullptr;
}

void FightingTriggersDispatcher::FlushTriggers(int triggered)
{
    for (auto& trigger : all_triggers) {
        trigger.Flush(Field, all_events[0]);
    }
}

void FightingTriggersDispatcher::AddTriggerAbilitiesFromUnit(ABActorFightingUnitBase* Unit) {
    for (auto& ability : Unit->UnitParameters->Abilities) {
        AddTriggerAbility(ability);
    }
}