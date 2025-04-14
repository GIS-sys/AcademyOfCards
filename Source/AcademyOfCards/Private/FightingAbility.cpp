// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "FightingAbilityGiveAbility.h"
#include "FightingAbilityDealDamage.h"
#include "FightingAbilityJump.h"
#include "FightingAbilityGetStats.h"
#include "FightingAbilityGiveStats.h"
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
		throw "FightingAbility::Build got unexpected Type: " + Type + " (ID: " + ID + ")";
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

void FightingAbility::OnMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo)
{
	if (When != ON_MOVE && When != ALWAYS) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnMove was triggered"), *ID, *Type);
	_OnAnything(OwnerUnit);
	_OnMove(OwnerUnit, CellFrom, CellTo);
}

void FightingAbility::OnSpawn(ABActorFightingUnitBase* OwnerUnit)
{
	if (When != INVOCATION && When != SPELL_CAST && When != ALWAYS) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnSpawn was triggered"), *ID, *Type);
	_OnAnything(OwnerUnit);
	_OnSpawn(OwnerUnit);
}

void FightingAbility::OnTurnEnd(ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner)
{
	if (When != ON_TURN_END && When != ALWAYS) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnTurnEnd was triggered"), *ID, *Type);
	_OnAnything(OwnerUnit);
	_OnTurnEnd(OwnerUnit, TurnEndedIsThisOwner);
}

void FightingAbility::OnAttackUnit(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim)
{
	if (When != ON_ATTACK && When != ALWAYS) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnAttackUnit was triggered"), *ID, *Type);
	_OnAnything(OwnerUnit);
	_OnAttackUnit(OwnerUnit, Victim);
}

void FightingAbility::OnGetAttacked(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker)
{
	if (When != ON_GET_ATTACKED && When != ALWAYS) return;
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnGetAttacked was triggered"), *ID, *Type);
	_OnAnything(OwnerUnit);
	_OnGetAttacked(OwnerUnit, Attacker);
}