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
	UnitParameters->Range = 1;
	UnitParameters->Attacks = 1;
	UnitParameters->Movement = 1;
	if (OpponentName == "Acolyte_easy") {
		UnitParameters->Power = 1;
		UnitParameters->Health = 20;
	} else if (OpponentName == "Goblins") {
		UnitParameters->Power = 5;
		UnitParameters->Health = 20;
	} else if (OpponentName == "Zombie") {
		UnitParameters->Power = 2;
		UnitParameters->Health = 30;
	} else if (OpponentName == "Werewolf") {
		UnitParameters->Power = 5;
		UnitParameters->Health = 50;
	} else {
		UnitParameters->Power = 2;
		UnitParameters->Health = 20;
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

FString ABActorFightingUnitBase::CanMove(ABActorFightingField* Field, ABActorFightingCellBase* Cell) {
	// Prepare
	std::map<FString, std::any> args;
	args["unit"] = this;
	args["is_occupied"] = Field->IsOccupied(Cell);
	args["move_cost"] = 1;
	args["dist_can"] = 1;
	args["dist_req"] = ABActorFightingCellBase::Distance(CurrentCell, Cell);
	args["is_controlled_by_correct_player"] = (Field->IsPlayerTurn == IsControlledByPlayer);
	// Ask abilities
	for (auto& trigger : Field->TriggersDispatcher.all_triggers)
		trigger.CanMove(this, Cell, args, Field);
	// Check yourself
	if (!std::any_cast<bool>(args["is_controlled_by_correct_player"])) return "Move your own Units";
	if (std::any_cast<bool>(args["is_occupied"])) return "The Cell is occupied";
	if (std::any_cast<ABActorFightingUnitBase*>(args["unit"])->UnitParameters->CurrentMovement < std::any_cast<int>(args["move_cost"])) return "Unit is out of movement";
	if (std::any_cast<int>(args["dist_can"]) < std::any_cast<int>(args["dist_req"])) return "Unit can't move that far";
	return "";
}

FString ABActorFightingUnitBase::CanAttack(ABActorFightingField* Field, ABActorFightingUnitBase* Victim) {
	// Prepare
	std::map<FString, std::any> args;
	args["attacker"] = this;
	args["victim"] = Victim;
	args["attack_cost"] = 1;
	args["range"] = UnitParameters->Range;
	args["distance"] = ABActorFightingCellBase::Distance(CurrentCell, Victim->CurrentCell);
	args["is_controlled_by_correct_player"] = (Field->IsPlayerTurn == IsControlledByPlayer);
	// Ask abilities
	for (auto& trigger : Field->TriggersDispatcher.all_triggers)
		trigger.CanAttack(this, Victim, args, Field);
	// Check yourself
	if (!std::any_cast<bool>(args["is_controlled_by_correct_player"])) return "Move your own Units";
	ABActorFightingUnitBase* PrAttacker = std::any_cast<ABActorFightingUnitBase*>(args["attacker"]);
	if (PrAttacker == std::any_cast<ABActorFightingUnitBase*>(args["victim"])) return "Can't attack yourself";
	if (PrAttacker->UnitParameters->CurrentAttacks < std::any_cast<int>(args["attack_cost"])) return "Unit is out of attacks";
	if (std::any_cast<int>(args["distance"]) > std::any_cast<int>(args["range"])) return "Unit can't attack that far";
	return "";
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