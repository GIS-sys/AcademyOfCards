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


void ABActorFightingUnitBase::Move(ABActorFightingField* Field, ABActorFightingCellBase* Cell, int MoveCost)
{
	UnitParameters->CurrentMovement -= MoveCost;
	MoveOverTimeTo(LocationOriginal, Cell->GetUnitLocation(), MOVING_TIME);
	CurrentCell = Cell;
}

void ABActorFightingUnitBase::TakeDamage(int Damage)
{
	UnitParameters->CurrentHealth -= Damage;
}

bool ABActorFightingUnitBase::CanMove(ABActorFightingField* Field, ABActorFightingCellBase* Cell) {
	// Prepare
	std::map<FString, std::any> args;
	args["unit"] = this;
	args["is_occupied"] = Field->IsOccupied(Cell);
	args["move_cost"] = 1;
	args["dist_can"] = 1;
	args["dist_req"] = ABActorFightingCellBase::Distance(CurrentCell, Cell);
	// Ask abilities
	for (auto& trigger : Field->TriggersDispatcher.all_triggers)
		trigger.CanMove(this, Cell, args, Field);
	// Check yourself
	if (std::any_cast<bool>(args["is_occupied"])) return false;
	if (std::any_cast<ABActorFightingUnitBase*>(args["unit"])->UnitParameters->CurrentMovement < std::any_cast<int>(args["move_cost"])) return false;
	if (std::any_cast<int>(args["dist_can"]) < std::any_cast<int>(args["dist_req"])) return false;
	return true;
}

bool ABActorFightingUnitBase::CanAttack(ABActorFightingField* Field, ABActorFightingUnitBase* Victim) {
	// Prepare
	std::map<FString, std::any> args;
	args["attacker"] = this;
	args["victim"] = Victim;
	args["attack_cost"] = 1;
	args["range"] = UnitParameters->Range;
	args["distance"] = ABActorFightingCellBase::Distance(CurrentCell, Victim->CurrentCell);
	// Ask abilities
	for (auto& trigger : Field->TriggersDispatcher.all_triggers)
		trigger.CanAttack(this, Victim, args, Field);
	// Check yourself
	ABActorFightingUnitBase* PrAttacker = std::any_cast<ABActorFightingUnitBase*>(args["attacker"]);
	if (PrAttacker == std::any_cast<ABActorFightingUnitBase*>(args["victim"])) return false;
	if (PrAttacker->UnitParameters->CurrentAttacks < std::any_cast<int>(args["attack_cost"])) return false;
	if (std::any_cast<int>(args["distance"]) > std::any_cast<int>(args["range"])) return false;
	return true;
}

bool ABActorFightingUnitBase::IsDead(ABActorFightingField* Field) {
	// Prepare
	std::map<FString, std::any> args;
	args["unit"] = this;
	args["health_threshold"] = 0;
	// Ask abilities
	for (auto& trigger : Field->TriggersDispatcher.all_triggers)
		trigger.IsDead(this, args, Field);
	// Check yourself
	ABActorFightingUnitBase* PrUnit = std::any_cast<ABActorFightingUnitBase*>(args["unit"]);
	if (PrUnit->UnitParameters->CurrentHealth <= std::any_cast<int>(args["health_threshold"])) return true;
	return false;
}