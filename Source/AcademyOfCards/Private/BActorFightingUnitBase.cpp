// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingUnitBase.h"

bool ABActorFightingUnitBase::Move(ABActorFightingField* Field, ABActorFightingCellBase* Cell)
{
	if (UnitParameters->CurrentMovement < ABActorFightingCellBase::Distance(Cell, CurrentCell)) return false;
	UnitParameters->CurrentMovement -= ABActorFightingCellBase::Distance(Cell, CurrentCell);
	MoveOverTimeTo(LocationOriginal, Cell->GetUnitLocation(), MOVING_TIME);
	ABActorFightingCellBase* CellFrom = CurrentCell;
	CurrentCell = Cell;
	for (const auto& Ability : UnitParameters->Abilities) {
		Ability->OnMove(Field, this, CellFrom, Cell);
	}
	return true;
}

void ABActorFightingUnitBase::OnSpawn(ABActorFightingField* Field)
{
	UnitParameters->ResetCurrent(true);
	for (const auto& Ability : UnitParameters->Abilities) {
		Ability->OnSpawn(Field, this);
	}
}

void ABActorFightingUnitBase::OnTurnEnd(ABActorFightingField* Field, bool TurnEndedIsThisOwner)
{
	if (!TurnEndedIsThisOwner) {
		UnitParameters->ResetCurrent();
	}
	for (const auto& Ability : UnitParameters->Abilities) {
		Ability->OnTurnEnd(Field, this, TurnEndedIsThisOwner);
	}
}

void ABActorFightingUnitBase::OnAttackUnit(ABActorFightingField* Field, ABActorFightingUnitBase* Victim)
{
	for (const auto& Ability : UnitParameters->Abilities) {
		Ability->OnAttackUnit(Field, this, Victim);
	}
}

void ABActorFightingUnitBase::OnGetAttacked(ABActorFightingField* Field, ABActorFightingUnitBase* Attacker)
{
	for (const auto& Ability : UnitParameters->Abilities) {
		Ability->OnGetAttacked(Field, this, Attacker);
	}
}

void ABActorFightingUnitBase::InitPlayerMy(ABActorFightingField* Field, ABActorFightingCellBase* Cell, const FPlayerStats* Stats)
{
	IsPlayer = true;
	LocationOriginal = Cell->GetUnitLocation();
	CurrentCell = Cell;
	IsControlledByPlayer = true;
	UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
	UnitParameters->Health = Stats->Health;
	UnitParameters->Movement = 1; // TODO player stats
	UnitParameters->Attacks = 1;
	UnitParameters->Power = 1;
	UnitParameters->Range = 1;
	OnSpawn(Field);
}

void ABActorFightingUnitBase::InitPlayerOpponent(ABActorFightingField* Field, FString OpponentName, ABActorFightingCellBase* Cell, FPlayerStats* Stats)
{
	IsPlayer = true;
	LocationOriginal = Cell->GetUnitLocation();
	CurrentCell = Cell;
	IsControlledByPlayer = false;
	UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
	if (OpponentName == "acolyte_easy") { // TODO opponent stats
		UnitParameters->Attacks = 1;
		UnitParameters->Health = 5;
		UnitParameters->Movement = 2;
		UnitParameters->Power = 1;
		UnitParameters->Range = 1;
	} else {
		UnitParameters->Attacks = 3;
		UnitParameters->Health = 1;
		UnitParameters->Movement = 2;
		UnitParameters->Power = 5;
		UnitParameters->Range = 3;
	}
	//UnitParameters = UnitParameters;
	OnSpawn(Field);
}