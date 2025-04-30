// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityDealDamage.h"
#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

void FightingAbilityDealDamage::_OnAttachAbility(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	DealDamage(Field, OwnerUnit);
}

void FightingAbilityDealDamage::_OnSpawn(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	DealDamage(Field, OwnerUnit);
}

void FightingAbilityDealDamage::DealDamage(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	for (auto& Unit : Field->ArrayUnits) {
		if (Unit->IsControlledByPlayer == OwnerUnit->IsControlledByPlayer) break;
		Field->AttackUnit(OwnerUnit, Unit);
		break;
	}
}