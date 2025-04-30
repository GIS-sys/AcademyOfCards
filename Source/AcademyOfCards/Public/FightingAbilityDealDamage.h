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
	void DealDamage(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit);

	virtual void _OnSpawn(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) override;
	virtual void _OnAttachAbility(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) override;
};
