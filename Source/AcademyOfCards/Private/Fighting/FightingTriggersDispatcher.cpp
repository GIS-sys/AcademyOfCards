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

    if (Field->UIManager.IsTriggerWaitingPlayerResponse() || all_events.empty()) return;

    UE_LOG(LogTemp, Warning, TEXT("Event is triggering: %s (%d total events)"), *all_events[0].ToDebugString(), all_events.size());
    TriggersDispatcherTrigger* trigger = CheckTriggers();
    if (trigger && triggered < MAX_TRIGGERS_PER_EVENT) {
        all_events.push_back(TriggersDispatcherEvent::MakeTriggerTriggered(trigger));
        UE_LOG(LogTemp, Warning, TEXT("    Triggering! (%s)"), *(trigger->ability->Type));
        trigger->Exec(Field, all_events[0]);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Event finished triggering: %s"), *all_events[0].ToDebugString());
    FlushTriggers();
    triggered = 0;
    all_events[0].Callback();
    history_events.push_back(all_events[0]);
    all_events.pop_front();
    Field->UIManager.LetActionsRegular();
}

TriggersDispatcherTrigger* FightingTriggersDispatcher::CheckTriggers()
{
    // triggered = 0
    //   i=0, no, triggered = 1, ?-
    //   i=1, no, triggered = 2, ?+
    //
    // <trigger [1] activated>
    //
    // triggered = 2
    // ...
    //   i = 2, no, trigered = 3, ?...
    int i = 0;
    UE_LOG(LogTemp, Warning, TEXT("  Checking triggers (total %d, triggered before %d)"), all_triggers.size(), triggered);
    for (auto& trigger : all_triggers) {
        if (i < triggered) {
            ++i;
            continue;
        }
        ++triggered;
        UE_LOG(LogTemp, Warning, TEXT("  Checking triggers (current %d, trigger %s, event %s)"), i, *(trigger.ability->Type), *all_events[0].ToDebugString());
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

void FightingTriggersDispatcher::AddTriggerAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability, bool add_after_current) {
    auto it = all_triggers.begin();
    if (add_after_current) {
        for (int i = 0; i < triggered; ++i) ++it;
    } else {
        it = all_triggers.end();
    }
    
    all_triggers.insert(it, TriggersDispatcherTrigger::MakeAbility(Unit, Ability));
}

void FightingTriggersDispatcher::AddEvent(TriggersDispatcherEvent Event) {
    all_events.push_back(Event);
}

void FightingTriggersDispatcher::DeleteTriggerAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability) {
    all_triggers.remove_if([&Ability, &Unit](const TriggersDispatcherTrigger& trigger) { return trigger.unit == Unit && trigger.ability == Ability; });
}






TriggersDispatcherEvent TriggersDispatcherEvent::MakeTriggerTriggered(TriggersDispatcherTrigger* new_trigger) {
    TriggersDispatcherEvent tde;
    tde.type = Type::TRIGGER; // 0 = trigger, 1 = ability, 2 = event
    tde.trigger = new_trigger;
    // tde.ability;
    // tde.ability_args;
    // tde.old_ability_args;
    // tde.event;
    // tde.event_args;
    // tde.old_event_args;
    // tde.CallbackFoo;
    return tde;
}

TriggersDispatcherEvent TriggersDispatcherEvent::MakeAbility(TriggersDispatcherEvent_EnumAbility enum_ability, std::map<FString, std::any> args, std::function<FString(std::map<FString, std::any>&)> callback) {
    TriggersDispatcherEvent tde;
    tde.type = Type::ABILITY; // 0 = trigger, 1 = ability, 2 = event
    // tde.trigger;
    tde.ability = enum_ability;
    tde.ability_args = args;
    tde.old_ability_args = args;
    // tde.event;
    // tde.event_args;
    // tde.old_event_args;
    tde.CallbackFoo = callback;
    return tde;
}

TriggersDispatcherEvent TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent enum_event, std::map<FString, std::any> args, std::function<FString(std::map<FString, std::any>&)> callback) {
    TriggersDispatcherEvent tde;
    tde.type = Type::EVENT; // 0 = trigger, 1 = ability, 2 = event
    // tde.trigger;
    // tde.ability;
    // tde.ability_args;
    // tde.old_ability_args;
    tde.event = enum_event;
    tde.event_args = args;
    tde.old_event_args = args;
    tde.CallbackFoo = callback;
    return tde;
}

void TriggersDispatcherEvent::Callback() {
    if (type == Type::TRIGGER)
        return;
    if (type == Type::ABILITY)
        CallbackFoo(ability_args);
    if (type == Type::EVENT)
        CallbackFoo(event_args);
}

FString TriggersDispatcherEvent::ToDebugString() const {
    if (type == Type::TRIGGER)
        return "Trigger";
    if (type == Type::ABILITY)
        return "Ability:" + TriggersDispatcherEvent_EnumAbility_ToString(ability);
    if (type == Type::EVENT)
        return "Event:" + TriggersDispatcherEvent_EnumEvent_ToString(event);
    return "UNK";
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

void TriggersDispatcherTrigger::CanMove(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell, std::map<FString, std::any>& args, ABActorFightingField* Field) {
    ability->CanMove(unit, Unit, Cell, args, Field);
}

void TriggersDispatcherTrigger::CanAttack(ABActorFightingUnitBase* Unit, ABActorFightingUnitBase* Victim, std::map<FString, std::any>& args, ABActorFightingField* Field) {
    ability->CanAttack(unit, Unit, Victim, args, Field);
}

void TriggersDispatcherTrigger::IsDead(ABActorFightingUnitBase* Unit, std::map<FString, std::any>& args, ABActorFightingField* Field) {
    ability->IsDead(unit, Unit, args, Field);
}

TriggersDispatcherTrigger TriggersDispatcherTrigger::MakeAbility(ABActorFightingUnitBase* Unit, TSharedPtr<FightingAbility> Ability) {
    TriggersDispatcherTrigger tdt;
    tdt.ability = Ability;
    tdt.unit = Unit;
    return tdt;
}
