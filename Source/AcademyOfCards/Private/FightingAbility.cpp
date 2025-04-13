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

FightingAbility FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments)
{
	// TODOIMPORTANT
	AdditionalArguments = BuildArguments;
	return *this;
}