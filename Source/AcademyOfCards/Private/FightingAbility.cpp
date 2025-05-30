// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "FightingAbilityGiveAbility.h"
#include "FightingAbilityDealDamage.h"
#include "FightingAbilityJump.h"
#include "FightingAbilityGetStats.h"
#include "FightingAbilityGiveStats.h"
#include "BActorFightingField.h"
#include "UMyGameInstance.h"

bool FightingAbilityCondition::Check(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) const { // TODO IMPORTANT
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
		/*} else if (Type == "Previous succed") {
			UE_LOG(LogTemp, Error, TEXT("UNKNOWN CONDITION TYPE (FightingAbilityCondition::Check) %s"), *Type);
			return true;
		} else if (Type == "attack_opponent_destroyed") {
			UE_LOG(LogTemp, Error, TEXT("UNKNOWN CONDITION TYPE (FightingAbilityCondition::Check) %s"), *Type);
			return true;*/
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

TSharedPtr<FightingAbility> FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments) const
{
	TSharedPtr<FightingAbility> AbilityBuilt = nullptr;
	if (Type == "Jump") { // TODO IMPORTANT
		AbilityBuilt = MakeShareable(new FightingAbilityJump());
	} else if (Type == "Get stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGetStats());
	/*} else if (Type == "Deal_damage") {
		AbilityBuilt = MakeShareable(new FightingAbilityDealDamage());
	} else if (Type == "Give ability") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveAbility());
	} else if (Type == "Give stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveStats());*/
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
	if (!AbilityBuilt->AdditionalArguments->TryGetStringField("when", WhenStr)) {
		if (!AbilityBuilt->Arguments->TryGetStringField("when", WhenStr)) WhenStr = "";
	}
	if (WhenStr == "Invocation") { // TODO IMPORTANT
		AbilityBuilt->When.insert(INVOCATION);
	//} else if (WhenStr == "on_attack") {
	//	AbilityBuilt->When = ON_ATTACK;
	//} else if (WhenStr == "spell_cast") {
	//	AbilityBuilt->When = SPELL_CAST;
	} else {
		//AbilityBuilt->When = ALWAYS;
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

	AbilityBuilt->_Build();
	return AbilityBuilt;
}