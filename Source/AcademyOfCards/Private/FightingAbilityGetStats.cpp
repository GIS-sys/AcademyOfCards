// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityGetStats.h"
#include "BActorFightingUnitBase.h"

void FightingAbilityGetStats::_Build() {
	if (!AdditionalArguments->TryGetNumberField("Health", DeltaHealth))
		if (!Arguments->TryGetNumberField("Health", DeltaHealth))
			DeltaHealth = 0;
	if (!AdditionalArguments->TryGetNumberField("Power", DeltaPower))
		if (!Arguments->TryGetNumberField("Power", DeltaPower))
			DeltaPower = 0;
}

bool CheckIsWhenApplicable(const WHEN& when, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) { // TODO IMPORTANT move to fighting ability
	if (when == WHEN::INVOCATION) {
		// Only react to event PLAYED
		if (Event.type != TriggersDispatcherEvent::Type::EVENT || Event.event != TriggersDispatcherEvent_EnumEvent::PLAYED_CARD) return false;
		// Only react to moving yourself
		ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(Event.event_args["created_unit"]);
		if (TargetUnit != OwnerUnit) return false;
		return true;
	} else {
		UE_LOG(LogTemp, Error, TEXT("ERROR UNKNOWN WHEN TYPE %d (CheckIsWhenApplicable)"), (int)when);
		return false;
	}
}

bool FightingAbilityGetStats::CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	// Only react to what we need
	bool applicable = false;
	for (const auto& when : When) {
		applicable = CheckIsWhenApplicable(when, Event, OwnerUnit);
		if (applicable) break;
	}
	return applicable;
}

void FightingAbilityGetStats::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	// {"card", PrCard}, { "cell", PrCell }, { "mana_cost", PrManaCost }, { "mana_gain", PrManaGain }, { "created_unit", NewUnit }
	OwnerUnit->UnitParameters->Health += DeltaHealth;
	OwnerUnit->UnitParameters->CurrentHealth += DeltaHealth;
	OwnerUnit->UnitParameters->Power += DeltaPower;
	OwnerUnit->UnitParameters->CurrentPower += DeltaPower;
}