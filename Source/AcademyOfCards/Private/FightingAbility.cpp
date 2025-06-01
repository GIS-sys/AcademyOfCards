// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "FightingAbilityGiveAbility.h"
#include "FightingAbilityDealDamage.h"
#include "FightingAbilityJump.h"
#include "FightingAbilityGetStats.h"
#include "FightingAbilityGiveStats.h"
#include "FightingAbilitySpellDeath.h"
#include "BActorFightingField.h"
#include "UMyGameInstance.h"

std::map<FString, std::any> FightingAbilityTarget::previous_args = {};

void FightingAbilityTarget::With(
	ABActorFightingField* Field,
	TriggersDispatcherEvent& Event,
	ABActorFightingUnitBase* OwnerUnit,
	std::function<void(const std::map<FString, std::any>&, ABActorFightingField*, TriggersDispatcherEvent&, ABActorFightingUnitBase*)> Callback
) const {
	FString Type;
	if (Data->HasField("type"))
		Type = Data->GetStringField("type");

	if (Type == "attack_target") {
		std::map<FString, std::any> args;
		auto& event = Field->TriggersDispatcher.all_events[0];
		if (event.type == event.EVENT && event.event == TriggersDispatcherEvent_EnumEvent::ATTACKED) {
			ABActorFightingUnitBase* Unit = std::any_cast<ABActorFightingUnitBase*>(event.event_args[FString("victim")]);
			args["unit"] = Unit;
			// TODO IMPORTANT check range and zone
			previous_args = args;
			Callback(args, Field, Event, OwnerUnit);
			return;
		} else {
			// Callback(args, Field, Event, OwnerUnit);
		}
	} else if (Type == "as_previous") {
		Callback(previous_args, Field, Event, OwnerUnit);
	} else if (Type == "select_enemy" || Type == "select_ally" || Type == "select_unit") {
		FightingUIManagerClickType WhatToChoose;
		WhatToChoose = FightingUIManagerClickType::OnUnit;

		Field->UIManager
			.Clear()
			->RegisterCallback(
				[this, Field, &Event, OwnerUnit, Callback, WhatToChoose, Type](FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
					// Dont choose anything - no callback
					if (cbt == FightingUIManagerClickType::OnOutside) {
						// Callback(args, Field, Event, OwnerUnit);
						Field->UIManager.TriggerDoesntNeedInput();
						Field->UIManager.LetActionsRegular();
						previous_args = {};
						std::map<FString, std::any> args;
						return FString();
					}
					// Chose unit - check things and apply damage
					std::map<FString, std::any> args;
					if (WhatToChoose == FightingUIManagerClickType::OnUnit) {
						if (unit->IsASpell) return FString("Don't choose Spell");
						// Check type
						if (Type == "select_enemy") {
							if (OwnerUnit->IsControlledByPlayer == unit->IsControlledByPlayer) {
								return FString("Choose Enemy unit");
							}
						} else if (Type == "select_ally") {
							if (OwnerUnit->IsControlledByPlayer != unit->IsControlledByPlayer) {
								return FString("Choose Ally unit");
							}
						} else if (Type == "select_unit") {
						} else {
							UE_LOG(LogTemp, Error, TEXT("FightingAbilityTarget::With got unknown type %s"), *Type);
						}
						// Check zone
						if (Data->HasField("zone")) {
							FString Zone = Data->GetStringField("zone");
							if (Zone == "line") {
								if (!ABActorFightingCellBase::AreOnTheLine(OwnerUnit->CurrentCell, unit->CurrentCell)) {
									return FString("Choose unit on the line");
								}
							} else if (Zone == "any") {
							} else {
								UE_LOG(LogTemp, Error, TEXT("FightingAbilityTarget::With got unknown zone %s"), *Zone);
							}
						}
						// Check range
						if (Data->HasField("range")) {
							int Range = (int)(Data->GetNumberField("range") + 0.5);
							if (ABActorFightingCellBase::Distance(OwnerUnit->CurrentCell, unit->CurrentCell) > Range) {
								return FString("Choose proper distance");
							}
						}
						args["unit"] = unit;
					}
					previous_args = args;
					Callback(args, Field, Event, OwnerUnit);
					Field->UIManager.TriggerDoesntNeedInput();
					Field->UIManager.LetActionsRegular();
					return FString();
				}, { WhatToChoose, FightingUIManagerClickType::OnOutside }
			)
			->TriggerNeedsInput();
	} else {
		previous_args = {};
		UE_LOG(LogTemp, Error, TEXT("FightingAbilityTarget::With got unknown type %s"), *Type);
		std::map<FString, std::any> args;
		Callback(args, Field, Event, OwnerUnit);
	}
}


bool FightingAbilityCondition::Check(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) const {
	if (Type == "Allied_familiar_died_this_turn") {
		for (auto it = Field->TriggersDispatcher.history_events.rbegin(); it != Field->TriggersDispatcher.history_events.rend(); ++it) {
			auto& event = *it;
			if (event.type == event.ABILITY && event.ability == TriggersDispatcherEvent_EnumAbility::PASSED_TURN) break;
			if (event.type != event.EVENT) continue;
			if (event.event == TriggersDispatcherEvent_EnumEvent::UNIT_DIED) {
				ABActorFightingUnitBase* Unit = std::any_cast<ABActorFightingUnitBase*>(event.event_args[FString("unit")]);
				if (OwnerUnit->IsControlledByPlayer == Unit->IsControlledByPlayer) {
					return true;
				}
				continue;
			}
		}
		return false;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("UNKNOWN CONDITION TYPE (FightingAbilityCondition::Check) %s"), *Type);
		return true;
	}
}




FightingAbility::FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
	ID = data->GetStringField("ID");
	Type = data->GetStringField("Type");
	Description = data->GetStringField("Description");
	Arguments = data->GetObjectField("Arguments");
}

TSharedPtr<FightingAbility> FightingAbility::FactoryBuildSpellDeath() {
	TSharedPtr<FightingAbility> AbilityBuilt = MakeShareable(new FightingAbilitySpellDeath());

	AbilityBuilt->ID = AbilityBuilt->Type;
	AbilityBuilt->Type = "__FightingAbilitySpellDeath";
	AbilityBuilt->When.insert(WHEN::INVOCATION);

	return AbilityBuilt;
}

TSharedPtr<FightingAbility> FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments) const
{
	TSharedPtr<FightingAbility> AbilityBuilt = nullptr;
	if (Type == "Jump") {
		AbilityBuilt = MakeShareable(new FightingAbilityJump());
	} else if (Type == "Get stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGetStats());
	} else if (Type == "Deal_damage") {
		AbilityBuilt = MakeShareable(new FightingAbilityDealDamage());
	} else if (Type == "Give stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveStats());
	} else if (Type == "Give ability") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveAbility());
	} else {
		// throw "FightingAbility::Build got unexpected Type: " + Type + " (ID: " + ID + ")";
		UE_LOG(LogTemp, Error, TEXT("ERROR UNKNOWN ABILITY TYPE %s (FightingAbility::Build)"), *Type);
		AbilityBuilt = MakeShareable(new FightingAbility());
	}

	AbilityBuilt->ID = ID;
	AbilityBuilt->Type = Type;
	AbilityBuilt->Description = Description;
	AbilityBuilt->Arguments = Arguments;
	AbilityBuilt->AdditionalArguments = BuildArguments;

	FString WhenStr;
	if (!AbilityBuilt->AdditionalArguments->TryGetStringField("when", WhenStr))
		if (!AbilityBuilt->Arguments->TryGetStringField("when", WhenStr))
			WhenStr = "";
	if (WhenStr == "Invocation" || WhenStr == "spell_cast") {
		AbilityBuilt->When.insert(WHEN::INVOCATION);
	} else if (WhenStr == "on_attack") {
		AbilityBuilt->When.insert(WHEN::ON_ATTACK);
	} else {
		UE_LOG(LogTemp, Error, TEXT("ERROR UNKNOWN WHEN s (FightingAbility::Build)"), *WhenStr);
	}

	if (AbilityBuilt->AdditionalArguments->HasField("condition") || AbilityBuilt->Arguments->HasField("condition")) {
		TSharedPtr<FJsonObject> ConditionsObject;
		if (AbilityBuilt->AdditionalArguments->HasField("condition"))
			ConditionsObject = AbilityBuilt->AdditionalArguments->TryGetField("condition")->AsObject();
		else
			ConditionsObject = AbilityBuilt->Arguments->TryGetField("condition")->AsObject();
		for (auto& [x, y] : ConditionsObject->Values) {
			AbilityBuilt->Conditions[x] = FightingAbilityCondition(x, y);
		}
	}

	if (AbilityBuilt->AdditionalArguments->HasField("target") || AbilityBuilt->Arguments->HasField("target")) {
		if (AbilityBuilt->AdditionalArguments->HasField("target"))
			AbilityBuilt->Target.Data = AbilityBuilt->AdditionalArguments->TryGetField("target")->AsObject();
		else
			AbilityBuilt->Target.Data = AbilityBuilt->Arguments->TryGetField("target")->AsObject();
	}

	AbilityBuilt->_Build();
	return AbilityBuilt;
}