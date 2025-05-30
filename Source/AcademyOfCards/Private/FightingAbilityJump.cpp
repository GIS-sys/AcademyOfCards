// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityJump.h"
#include "BActorFightingUnitBase.h"
#include "BActorFightingField.h"

void FightingAbilityJump::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	// Only react to event MOVE
	if (Event.type != TriggersDispatcherEvent::Type::EVENT || Event.event != TriggersDispatcherEvent_EnumEvent::MOVE) return;
	// Only react to moving yourself
	ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(Event.event_args["unit"]);
	if (TargetUnit != OwnerUnit) return;

	ABActorFightingCellBase* TargetFromCell = std::any_cast<ABActorFightingCellBase*>(Event.event_args["from_cell"]);
	ABActorFightingCellBase* TargetCell = std::any_cast<ABActorFightingCellBase*>(Event.event_args["cell"]);

	if (Event.event == TriggersDispatcherEvent_EnumEvent::MOVE) {
		// {"unit", Unit}, { "from_cell", Unit->CurrentCell }, { "cell", Cell }, { "move_cost", ABActorFightingCellBase::Distance(Cell, Unit->CurrentCell) }, {"proceed", true}, {"result", FString("")}
		int Distance = ABActorFightingCellBase::Distance(TargetFromCell, TargetCell);
		if (Distance > 2) {
			Event.event_args["move_cost"] = 1;
			if (std::any_cast<bool>(Event.event_args["proceed"])) {
				Event.event_args["proceed"] = false;
				Event.event_args["result"] = FString("Can't jump more than 2 cells");
			}
		}
	}
}

void FightingAbilityJump::CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell, std::map<FString, std::any>& args, ABActorFightingField* Field) {
	// unit, is_occupied, move_cost, dist_can, dist_req
	// Only react to moving yourself
	ABActorFightingUnitBase* TargetUnit = std::any_cast<ABActorFightingUnitBase*>(args["unit"]);
	if (TargetUnit != OwnerUnit) return;

	args["move_cost"] = 1;
	args["dist_can"] = 2;
}