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
    Clear();
    LetActionsRegular();
}

bool FightingUIManager::IsWaitingPlayerResponse() const {
    return wait;
}

void FightingUIManager::WaitForInput() {
    wait = true;
}

void FightingUIManager::Clear() {
    wait = false;
    Callbacks.clear();
}

bool FightingUIManager::ClickedOnCell(ABActorFightingCellBase* target) {
    return Callbacks[FightingUIManagerClickType::OnCell](FightingUIManagerClickType::OnCell, target, nullptr, "", nullptr);
}

bool FightingUIManager::ClickedOnUnit(ABActorFightingUnitBase* target) {
    return Callbacks[FightingUIManagerClickType::OnUnit](FightingUIManagerClickType::OnUnit, nullptr, target, "", nullptr);
}

bool FightingUIManager::ClickedOnAbility(FString target) {
    return Callbacks[FightingUIManagerClickType::OnAbility](FightingUIManagerClickType::OnAbility, nullptr, nullptr, target, nullptr);
}

bool FightingUIManager::ClickedOnCard(ABActorFightingCard* target) {
    return Callbacks[FightingUIManagerClickType::OnCard](FightingUIManagerClickType::OnCard, nullptr, nullptr, "", target);
}

bool FightingUIManager::ClickedOnPassTurn() {
    return Callbacks[FightingUIManagerClickType::OnPassTurn](FightingUIManagerClickType::OnPassTurn, nullptr, nullptr, "", nullptr);
}

void FightingUIManager::RegisterCallback(CallbackType callback_foo, std::vector<FightingUIManagerClickType> callback_types) {
    for (auto cbt : callback_types) {
        Callbacks[cbt] = callback_foo;
    }
}

void FightingUIManager::LetActionsRegular() {
    // TODO IMPORTANT
    UE_LOG(LogTemp, Error, TEXT("Let actions regular"));
    RegisterCallback(
        [](FightingUIManagerClickType cbt, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, FString ability, ABActorFightingCard* card) {
            UE_LOG(LogTemp, Error, TEXT("Callback!"));
            return true;
        },
        {
            FightingUIManagerClickType::OnCell,
            FightingUIManagerClickType::OnUnit,
            FightingUIManagerClickType::OnAbility,
            FightingUIManagerClickType::OnCard,
            FightingUIManagerClickType::OnPassTurn
        }
    );
}