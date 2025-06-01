// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityGiveStats.h"
#include "BActorFightingUnitBase.h"
#include "BActorfightingField.h"

void FightingAbilityGiveStats::_Build() {
	if (!AdditionalArguments->TryGetNumberField("Health", DeltaHealth))
		if (!Arguments->TryGetNumberField("Health", DeltaHealth))
			DeltaHealth = 0;
	if (!AdditionalArguments->TryGetNumberField("Power", DeltaPower))
		if (!Arguments->TryGetNumberField("Power", DeltaPower))
			DeltaPower = 0;
}

void FightingAbilityGiveStats::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	Target.With(Field, Event, OwnerUnit, [&](const std::map<FString, std::any>& args, ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
		if (args.find("unit") == args.end()) throw std::exception("FightingAbilityGiveStats needs a unit as a target");
		ABActorFightingUnitBase* ChosenUnit = std::any_cast<ABActorFightingUnitBase*>(args.find("unit")->second);

		ChosenUnit->UnitParameters->Health += DeltaHealth; // TODO IMPORTANT add event
		ChosenUnit->UnitParameters->CurrentHealth += DeltaHealth;
		ChosenUnit->UnitParameters->Power += DeltaPower;
		ChosenUnit->UnitParameters->CurrentPower += DeltaPower;
		if (ChosenUnit->UnitParameters->Power < 0) ChosenUnit->UnitParameters->Power = 0;
		if (ChosenUnit->UnitParameters->CurrentPower < 0) ChosenUnit->UnitParameters->CurrentPower = 0;
	});
}