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
	virtual bool CheckEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;
	virtual void ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) override;

	virtual void CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell, std::map<FString, std::any>& args, ABActorFightingField* Field) override;
};
