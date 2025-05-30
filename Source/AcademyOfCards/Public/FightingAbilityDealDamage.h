// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingAbility.h"

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbilityDealDamage : public FightingAbility
{
protected:
	int Damage = 0;

	virtual void _Build() override;

	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
};
