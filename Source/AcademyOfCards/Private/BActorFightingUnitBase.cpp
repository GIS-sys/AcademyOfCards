// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingUnitBase.h"

bool ABActorFightingUnitBase::Move(ABActorFightingCellBase* Cell)
{
	if (UnitParameters->CurrentMovement < ABActorFightingCellBase::Distance(Cell, CurrentCell)) return false;
	UnitParameters->CurrentMovement -= ABActorFightingCellBase::Distance(Cell, CurrentCell);
	MoveOverTimeTo(LocationOriginal, Cell->GetUnitLocation(), MOVING_TIME);
	CurrentCell = Cell;
	return true;
}

void ABActorFightingUnitBase::OnSpawn()
{
	UnitParameters->ResetCurrent();
}

void ABActorFightingUnitBase::OnTurnEnd(bool TurnEndedIsThisOwner)
{
	if (!TurnEndedIsThisOwner) {
		UnitParameters->ResetCurrent();
	}
}