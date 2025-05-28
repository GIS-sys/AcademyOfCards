// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighting/FightingUIManager.h"
#include "BActorFightingField.h"

FightingUIManager::FightingUIManager()
{
}

FightingUIManager::~FightingUIManager()
{
}

void FightingUIManager::Init(ABActorFightingField* NewField)
{
    Field = NewField;
    LetActionsRegular();
}
