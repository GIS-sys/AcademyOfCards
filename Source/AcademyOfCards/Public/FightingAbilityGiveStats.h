// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FightingAbility.h"

/**
 * 
 */
class ACADEMYOFCARDS_API FightingAbilityGiveStats : public FightingAbility
{
	// TODOIMPORTANT
protected:
	int DeltaHealth;
	int DeltaPower;

	FString TargetType;
	int TargetRange;

	virtual void _Build() override;
	virtual void _OnAnything(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit);
};
