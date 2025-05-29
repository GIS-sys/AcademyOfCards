// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighting/FightingTriggersDispatcher.h"
#include "BActorFightingField.h"
#include "FightingAbility.h"

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

    TriggersDispatcherTrigger* trigger = CheckTriggers();
    if (trigger && triggered < MAX_TRIGGERS_PER_EVENT) {
        ++triggered;
        all_events.push_back(TriggersDispatcherEvent::MakeTriggerTriggered(trigger));
        trigger->Exec(Field, all_events[0]);
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("Event finished triggering: %s"), *TriggersDispatcherEvent_EnumEvent_ToString(all_events[0].event));
    FlushTriggers();
    triggered = 0;
    all_events[0].Callback();
    all_events.pop_front();
    Field->UIManager.LetActionsRegular();
}

TriggersDispatcherTrigger* FightingTriggersDispatcher::CheckTriggers()
{
    int i = 0;
    UE_LOG(LogTemp, Error, TEXT("Checking triggers (total %d)"), all_triggers.size());
    for (auto& trigger : all_triggers) {
        if (trigger.Check(Field, all_events[0])) return &trigger;
        ++i;
    }
    return nullptr;
}

void FightingTriggersDispatcher::FlushTriggers()
{
    for (auto& trigger : all_triggers) {
        trigger.Flush(Field, all_events[0]);
    }
}

void FightingTriggersDispatcher::AddTriggerAbilitiesFromUnit(ABActorFightingUnitBase* Unit) {
    for (auto& Ability : Unit->UnitParameters->Abilities) {
        AddTriggerAbility(Unit, Ability);
    }
}

void FightingTriggersDispatcher::AddTriggerAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability) {
    all_triggers.push_back(TriggersDispatcherTrigger::MakeAbility(Unit, Ability));
}

void FightingTriggersDispatcher::AddEvent(TriggersDispatcherEvent Event) {
    all_events.push_back(Event);
}

void DeleteEventAbility(TSharedPtr<FightingAbility> Ability) {
    // TODO IMPORTANT
}





TriggersDispatcherEvent TriggersDispatcherEvent::MakeTriggerTriggered(TriggersDispatcherTrigger* trigger) {
    TriggersDispatcherEvent tde;
    tde.type = 0;
    tde.trigger = trigger;
    return tde;
}

TriggersDispatcherEvent TriggersDispatcherEvent::MakeAbility(TriggersDispatcherEvent_EnumAbility enum_ability, std::map<FString, std::any> args) {
    TriggersDispatcherEvent tde;
    tde.type = 1;
    tde.ability = enum_ability;
    tde.ability_args = args;
    return tde;
}

TriggersDispatcherEvent TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent enum_event, std::map<FString, std::any> args, std::function<FString(std::map<FString, std::any>)> results) {
    // TODO IMPORTANT
    TriggersDispatcherEvent tde;
    tde.type = 2;
    tde.event = enum_event;
    tde.event_args = args;
    tde.old_event_args = args;
    return tde;
}






bool TriggersDispatcherTrigger::Check(ABActorFightingField* Field, TriggersDispatcherEvent Event) {
    return ability->CheckEvent(Field, Event, unit);
}

void TriggersDispatcherTrigger::Exec(ABActorFightingField* Field, TriggersDispatcherEvent Event) {
    return ability->ExecEvent(Field, Event, unit);
}

void TriggersDispatcherTrigger::Flush(ABActorFightingField* Field, TriggersDispatcherEvent Event) {
    return ability->FlushEvent(Field, Event, unit);
}

TriggersDispatcherTrigger TriggersDispatcherTrigger::MakeAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability) {
    TriggersDispatcherTrigger tdt;
    tdt.ability = Ability;
    tdt.unit = Unit;
    return tdt;
}
