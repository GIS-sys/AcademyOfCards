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
		// {"unit", Unit}, { "from_cell", Unit->CurrentCell }, {"cell", Cell}, {"proceed", true}, {"result", FString("")}
		int Distance = ABActorFightingCellBase::Distance(TargetFromCell, TargetCell);
		if (Distance > 2) {
			if (std::any_cast<bool>(Event.event_args["proceed"])) {
				Event.event_args["proceed"] = false;
				Event.event_args["result"] = FString("Can't jump more than 2 cells");
			}
		}
	}
}

bool FightingAbilityJump::CanMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* Cell, bool can_move, ABActorFightingField* Field) {
	return can_move || (ABActorFightingCellBase::Distance(OwnerUnit->CurrentCell, Cell) <= 2);
}