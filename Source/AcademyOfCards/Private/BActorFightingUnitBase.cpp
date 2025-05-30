// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingUnitBase.h"

void ABActorFightingUnitBase::InitPlayerMy(ABActorFightingField* Field, ABActorFightingCellBase* Cell, const FPlayerStats* Stats)
{
	IsPlayer = true;
	LocationOriginal = Cell->GetUnitLocation();
	CurrentCell = Cell;
	IsControlledByPlayer = true;
	UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
	UnitParameters->Health = Stats->Health;
	UnitParameters->Movement = 1;
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

void ABActorFightingUnitBase::OnSpawn(ABActorFightingField* Field)
{
	UnitParameters->ResetCurrent(true);
}

void ABActorFightingUnitBase::OnTurnEnd(ABActorFightingField* Field, bool TurnEndedIsThisOwner)
{
	if (!TurnEndedIsThisOwner) {
		UnitParameters->ResetCurrent();
	}
}


void ABActorFightingUnitBase::Move(ABActorFightingField* Field, ABActorFightingCellBase* Cell)
{
	UnitParameters->CurrentMovement -= ABActorFightingCellBase::Distance(Cell, CurrentCell);
	MoveOverTimeTo(LocationOriginal, Cell->GetUnitLocation(), MOVING_TIME);
	CurrentCell = Cell;
}

void ABActorFightingUnitBase::TakeDamage(int Damage)
{
	UnitParameters->CurrentHealth -= Damage;
}

/*if (IsOccupied(Cell)) return "Cell is occupied"; and distance and acan move still*/
FString ABActorFightingUnitBase::CanMove(ABActorFightingCellBase* Cell) { // TODO IMPORTANT
	return "";
}

/*if (Attacker == Victim) return "Units cannot attack themselves";
if (Attacker->UnitParameters->CurrentAttacks <= 0) return "Unit is already out of attacks for this turn";
int Distance = ABActorFightingCellBase::Distance(Attacker->CurrentCell, Victim->CurrentCell);
if (Distance > Attacker->UnitParameters->Range) return;*/
FString ABActorFightingUnitBase::CanAttack(ABActorFightingUnitBase* Victim) { // TODO IMPORTANT
	return "";
}

// if (Unit->UnitParameters->CurrentHealth <= 0) {
FString ABActorFightingUnitBase::IsDead() { // TODO IMPORTANT
	return "";
}