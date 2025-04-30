// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityGiveStats.h"
#include "BActorFightingUnitBase.h"
#include "BActorfightingField.h"

void FightingAbilityGiveStats::_Build() {
	if (!AdditionalArguments->TryGetNumberField("Health", DeltaHealth)) {
		if (!Arguments->TryGetNumberField("Health", DeltaHealth)) DeltaHealth = 0;
	}
	if (!AdditionalArguments->TryGetNumberField("Power", DeltaPower)) {
		if (!Arguments->TryGetNumberField("Power", DeltaPower)) DeltaPower = 0;
	}

	const TSharedPtr<FJsonObject>* TargetObject;
	if (!AdditionalArguments->TryGetObjectField("target", TargetObject)) {
		if (!Arguments->TryGetObjectField("target", TargetObject)) TargetObject = nullptr;
	}
	if (TargetObject == nullptr) {
		TargetType = "";
		TargetRange = 0;
	} else {
		if (!(*TargetObject)->TryGetStringField("type", TargetType)) TargetType = "";
		if (!(*TargetObject)->TryGetNumberField("range", TargetRange)) TargetRange = 0;
	}
}

void FightingAbilityGiveStats::_OnAnything(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	ABActorFightingUnitBase* TargetUnit;
	if (TargetType == "select_ally") {
		TargetUnit = Field->GetRandomUnitNear(OwnerUnit->CurrentCell, TargetRange);
	} else {
		return;
		//throw "Unexpected Target Type"; // TODO
	}
	
	TargetUnit->UnitParameters->Health += DeltaHealth;
	TargetUnit->UnitParameters->CurrentHealth += DeltaHealth;
	TargetUnit->UnitParameters->Power += DeltaPower;
	TargetUnit->UnitParameters->CurrentPower += DeltaPower;
}