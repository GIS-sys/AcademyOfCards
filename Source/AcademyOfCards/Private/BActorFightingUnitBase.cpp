// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

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
	InitUnit();
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
	InitUnit();
}

void ABActorFightingUnitBase::InitUnit()
{
	UnitParameters->ResetCurrent(true);
}

void ABActorFightingUnitBase::ResetOnTurnEnd(bool TurnEndedIsThisOwner)
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

bool ABActorFightingUnitBase::CanMove(ABActorFightingField* Field, ABActorFightingCellBase* Cell) { // TODO IMPORTANT
	bool can_move = true;
	// Check yourself
	int Distance = ABActorFightingCellBase::Distance(CurrentCell, Cell);
	if (Field->IsOccupied(Cell))
		can_move = false;
	else if (UnitParameters->CurrentMovement <= 0)
		can_move = false;
	else if (Distance > UnitParameters->CurrentMovement)
		can_move = false;
	// Ask abilities
	for (auto& ability : UnitParameters->Abilities)
		can_move = ability->CanMove(this, Cell, can_move, Field);
	// Return
	return can_move;
}

bool ABActorFightingUnitBase::CanAttack(ABActorFightingField* Field, ABActorFightingUnitBase* Victim) { // TODO IMPORTANT
	bool can_attack = true;
	// Check yourself
	int Distance = ABActorFightingCellBase::Distance(CurrentCell, Victim->CurrentCell);
	if (this == Victim)
		can_attack = false;
	else if (UnitParameters->CurrentAttacks <= 0)
		can_attack = false;
	else if (Distance > UnitParameters->Range)
		can_attack = false;
	// Ask abilities
	for (auto& ability : UnitParameters->Abilities)
		can_attack = ability->CanAttack(this, Victim, can_attack, Field);
	// Return
	return can_attack;
}

bool ABActorFightingUnitBase::IsDead(ABActorFightingField* Field) { // TODO IMPORTANT
	bool is_dead = false;
	// Check yourself
	if (UnitParameters->CurrentHealth <= 0)
		is_dead = true;
	// Ask abilities
	for (auto& ability : UnitParameters->Abilities)
		is_dead = ability->IsDead(this, is_dead, Field);
	// Return
	return is_dead;
}