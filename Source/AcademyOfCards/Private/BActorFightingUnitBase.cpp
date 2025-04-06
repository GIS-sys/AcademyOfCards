// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingUnitBase.h"


void ABActorFightingUnitBase::Move(ABActorFightingCellBase* Cell)
{
	MoveOverTimeTo(LocationOriginal, Cell->GetUnitLocation(), MOVING_TIME);
}