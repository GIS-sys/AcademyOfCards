// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityJump.h"
#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

void FightingAbilityJump::_OnSpawn(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	OwnerUnit->UnitParameters->CurrentMovement += std::max(0, 2 - OwnerUnit->UnitParameters->Movement);
	OwnerUnit->UnitParameters->Movement = 2;
}