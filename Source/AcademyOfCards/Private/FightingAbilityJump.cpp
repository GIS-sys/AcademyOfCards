// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityJump.h"
#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

void FightingAbilityJump::_OnAnything(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) {
	OwnerUnit->UnitParameters->Movement = 2;
}