// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingUnitParameters.h"

void UFightingUnitParameters::ResetCurrent(bool on_spawn)
{
	CurrentPower = Power;
	CurrentMovement = Movement;
	CurrentAttacks = Attacks;
	CurrentRange = Range;
	if (on_spawn) {
		CurrentHealth = Health;
		CurrentMovement = 0;
		CurrentAttacks = 0;
	}
}