// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilitySpellDeath.h"
#include "BActorFightingUnitBase.h"
#include "BActorfightingField.h"


void FightingAbilitySpellDeath::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
    Field->DeleteUnit(OwnerUnit);
}