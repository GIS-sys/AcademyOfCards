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
	int DeltaHealth;
	int DeltaPower;

	virtual void _Build() override;

	virtual bool CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
};
