// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "FightingAbilityGiveAbility.h"
#include "FightingAbilityDealDamage.h"
#include "FightingAbilityJump.h"
#include "FightingAbilityGetStats.h"
#include "FightingAbilityGiveStats.h"
#include "BActorFightingField.h"
#include "UMyGameInstance.h"

FightingAbility::FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
	ID = data->GetStringField("ID");
	Type = data->GetStringField("Type");
	Description = data->GetStringField("Description");
	Arguments = data->GetObjectField("Arguments");
}

TSharedPtr<FightingAbility> FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments) const
{
	// TODOIMPORTANT
	TSharedPtr<FightingAbility> AbilityBuilt = nullptr;
	if (Type == "Give ability") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveAbility());
	} else if (Type == "Deal_damage") {
		AbilityBuilt = MakeShareable(new FightingAbilityDealDamage());
	} else if (Type == "Jump") {
		AbilityBuilt = MakeShareable(new FightingAbilityJump());
	} else if (Type == "Get stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGetStats());
	} else if (Type == "Give stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveStats());
	} else {
		// throw "FightingAbility::Build got unexpected Type: " + Type + " (ID: " + ID + ")"; // TODO IMPORTANT
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
	AbilityBuilt->When = ALWAYS;
	if (WhenStr == "Invocation") {
		AbilityBuilt->When = INVOCATION;
	}
	else if (WhenStr == "on_attack") {
		AbilityBuilt->When = ON_ATTACK;
	}
	else if (WhenStr == "spell_cast") {
		AbilityBuilt->When = SPELL_CAST;
	}

	AbilityBuilt->_Build();
	return AbilityBuilt;
}

bool FightingAbility::CheckCondition(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit) const { // TODO IMPORTANT
	return true;
}




void FightingAbility::OnMove(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo)
{
	if (When != ON_MOVE && When != ALWAYS) return;
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnMove was triggered"), *ID, *Type);
	_OnAnything(Field, OwnerUnit);
	_OnMove(Field, OwnerUnit, CellFrom, CellTo);
}

void FightingAbility::OnSpawn(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit)
{
	if (When != INVOCATION && When != SPELL_CAST && When != ALWAYS) return;
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnSpawn was triggered"), *ID, *Type);
	_OnAnything(Field, OwnerUnit);
	_OnSpawn(Field, OwnerUnit);
}

void FightingAbility::OnTurnEnd(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner)
{
	if (When != ON_TURN_END && When != ALWAYS) return;
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnTurnEnd was triggered"), *ID, *Type);
	_OnAnything(Field, OwnerUnit);
	_OnTurnEnd(Field, OwnerUnit, TurnEndedIsThisOwner);
}

void FightingAbility::OnAttackUnit(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim)
{
	if (When != ON_ATTACK && When != ALWAYS) return;
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnAttackUnit was triggered"), *ID, *Type);
	_OnAnything(Field, OwnerUnit);
	_OnAttackUnit(Field, OwnerUnit, Victim);
}

void FightingAbility::OnGetAttacked(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker)
{
	if (When != ON_GET_ATTACKED && When != ALWAYS) return;
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnGetAttacked was triggered"), *ID, *Type);
	_OnAnything(Field, OwnerUnit);
	_OnGetAttacked(Field, OwnerUnit, Attacker);
}

void FightingAbility::OnAttachAbility(ABActorFightingField* Field, ABActorFightingUnitBase* OwnerUnit)
{
	if (!CheckCondition(Field, OwnerUnit)) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnAttachAbility was triggered"), *ID, *Type);
	_OnAttachAbility(Field, OwnerUnit);
}