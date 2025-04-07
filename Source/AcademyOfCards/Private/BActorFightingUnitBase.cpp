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

void ABActorFightingUnitBase::InitPlayerMy(ABActorFightingCellBase* Cell, const FPlayerStats* Stats)
{
	IsPlayer = true;
	LocationOriginal = Cell->GetUnitLocation();
	CurrentCell = Cell;
	IsControlledByPlayer = true;
	UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
	UnitParameters->Health = Stats->Health;
	UnitParameters->Movement = 1; // TODO player stats
	OnSpawn();
}

void ABActorFightingUnitBase::InitPlayerOpponent(FString OpponentName, ABActorFightingCellBase* Cell, FPlayerStats* Stats)
{
	IsPlayer = true;
	LocationOriginal = Cell->GetUnitLocation();
	CurrentCell = Cell;
	IsControlledByPlayer = false;
	UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
	if (OpponentName == "acolyte_easy") { // TODO opponent stats
		UnitParameters->Attacks = 1;
		UnitParameters->Health = 10;
		UnitParameters->Movement = 2;
		UnitParameters->Power = 1;
		UnitParameters->Range = 1;
	} else {
		UnitParameters->Attacks = 3;
		UnitParameters->Health = 50;
		UnitParameters->Movement = 2;
		UnitParameters->Power = 5;
		UnitParameters->Range = 3;
	}
	//UnitParameters = UnitParameters;
	OnSpawn();
}