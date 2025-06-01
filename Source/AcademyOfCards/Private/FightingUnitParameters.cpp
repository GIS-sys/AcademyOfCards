// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingUnitParameters.h"

void UFightingUnitParameters::ResetCurrent(bool full)
{
	if (full) {
		CurrentHealth = Health;
	}
	CurrentPower = Power;
	CurrentMovement = Movement;
	CurrentAttacks = Attacks;
	CurrentRange = Range;
}