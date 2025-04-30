// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityGetStats.h"
#include "BActorFightingUnitBase.h"

void FightingAbilityGetStats::_Build() {
	if (!AdditionalArguments->TryGetNumberField("Health", DeltaHealth)) {
		if (!Arguments->TryGetNumberField("Health", DeltaHealth)) DeltaHealth = 0;
	}
	if (!AdditionalArguments->TryGetNumberField("Power", DeltaPower)) {
		if (!Arguments->TryGetNumberField("Power", DeltaPower)) DeltaPower = 0;
	}
}

void FightingAbilityGetStats::_OnAnything(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	OwnerUnit->UnitParameters->Health += DeltaHealth;
	OwnerUnit->UnitParameters->CurrentHealth += DeltaHealth;
	OwnerUnit->UnitParameters->Power += DeltaPower;
	OwnerUnit->UnitParameters->CurrentPower += DeltaPower;
}