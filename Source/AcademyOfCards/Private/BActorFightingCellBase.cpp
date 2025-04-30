// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingCellBase.h"

FVector ABActorFightingCellBase::GetUnitLocation()
{
	return LocationOriginal + FVector(0.0, 0.0, 50.0);
}