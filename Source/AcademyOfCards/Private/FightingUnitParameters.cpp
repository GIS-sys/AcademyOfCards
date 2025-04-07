// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingUnitParameters.h"

void UFightingUnitParameters::ResetCurrent()
{
	CurrentMovement = Movement;
	CurrentHealth = Health;
	//CurrentPower = Power; TODO
	CurrentPower += 1;
}