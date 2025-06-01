// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityDealDamage.h"
#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

void FightingAbilityDealDamage::_Build() {
	if (!AdditionalArguments->TryGetNumberField("damage", Damage))
		if (!Arguments->TryGetNumberField("damage", Damage))
			Damage = 0;
}

void FightingAbilityDealDamage::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	Target.With(Field, Event, OwnerUnit, [&](const std::map<FString, std::any>& args, ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
		if (args.find("unit") == args.end()) throw std::exception("FightingAbilityDealDamage needs a unit as a target");
		ABActorFightingUnitBase* ChosenUnit = std::any_cast<ABActorFightingUnitBase*>(args.find("unit")->second);

		Field->DealDamageWithEvent(OwnerUnit, ChosenUnit, Damage);
	});
}