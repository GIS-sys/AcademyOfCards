// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "UMyGameInstance.h"

FightingAbility::FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
	ID = data->GetStringField("ID");
	Type = data->GetStringField("Type");
	Description = data->GetStringField("Description");
	Arguments = data->GetObjectField("Arguments");
}

FightingAbility::~FightingAbility()
{
}

TSharedPtr<FightingAbility> FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments)
{
	// TODOIMPORTANT
	AdditionalArguments = BuildArguments;
	return MakeShareable(this);
}

void FightingAbility::OnMove(ABActorFightingUnitBase* OwnerUnit, ABActorFightingCellBase* CellFrom, ABActorFightingCellBase* CellTo)
{
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnMove was triggered"), *ID, *Type);
}

void FightingAbility::OnSpawn(ABActorFightingUnitBase* OwnerUnit)
{
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnSpawn was triggered"), *ID, *Type);
}

void FightingAbility::OnTurnEnd(ABActorFightingUnitBase* OwnerUnit, bool TurnEndedIsThisOwner)
{
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnTurnEnd was triggered"), *ID, *Type);
}

void FightingAbility::OnAttackUnit(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Victim)
{
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnAttackUnit was triggered"), *ID, *Type);
}

void FightingAbility::OnGetAttacked(ABActorFightingUnitBase* OwnerUnit, ABActorFightingUnitBase* Attacker)
{
	UE_LOG(LogTemp, Error, TEXT("Ability %s(%s)::OnGetAttacked was triggered"), *ID, *Type);
}