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
	virtual void _OnAnything(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) override;
};
