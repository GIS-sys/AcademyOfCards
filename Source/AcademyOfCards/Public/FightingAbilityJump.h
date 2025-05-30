// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingAbility.h"
class ABActorFightingField;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbilityJump : public FightingAbility
{
protected:
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
	virtual bool CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* Cell, bool can_move, ABActorFightingField* Field) override;
};
