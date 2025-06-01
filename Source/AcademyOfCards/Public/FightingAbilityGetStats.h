// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingAbility.h"

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbilityGetStats : public FightingAbility
{
protected:
	int DeltaHealth = 0;
	int DeltaPower = 0;

	virtual void _Build() override;

	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
};
