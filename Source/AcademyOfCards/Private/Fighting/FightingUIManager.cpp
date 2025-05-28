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
    state = {};
    LetActionsRegular();
}

bool FightingUIManager::IsWaitingPlayerResponse() const {
    return wait;
}

void FightingUIManager::WaitForInput() {
    wait = true;
}

FightingUIManager* FightingUIManager::Clear() {
    wait = false;
    Callbacks.clear();
    return this;
}

FString FightingUIManager::ClickedOnCell(ABActorFightingCellBase* target) {
    return Callbacks[FightingUIManagerClickType::OnCell](FightingUIManagerClickType::OnCell, this, target, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnUnit(ABActorFightingUnitBase* target) {
    return Callbacks[FightingUIManagerClickType::OnUnit](FightingUIManagerClickType::OnUnit, this, nullptr, target, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target) {
    return Callbacks[FightingUIManagerClickType::OnAbility](FightingUIManagerClickType::OnAbility, this, nullptr, nullptr, target, nullptr);
}

FString FightingUIManager::ClickedOnCard(ABActorFightingCard* target) {
    return Callbacks[FightingUIManagerClickType::OnCard](FightingUIManagerClickType::OnCard, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, target);
}

FString FightingUIManager::ClickedOnPassTurn() {
    return Callbacks[FightingUIManagerClickType::OnPassTurn](FightingUIManagerClickType::OnPassTurn, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FightingUIManager* FightingUIManager::RegisterCallback(CallbackType callback_foo, std::vector<FightingUIManagerClickType> click_types) {
    for (auto cbt : click_types) {
        Callbacks[cbt] = callback_foo;
    }
    return this;
}





std::vector<FightingUIManagerClickType> all_click_types{
    FightingUIManagerClickType::OnCell,
    FightingUIManagerClickType::OnUnit,
    FightingUIManagerClickType::OnAbility,
    FightingUIManagerClickType::OnCard,
    FightingUIManagerClickType::OnPassTurn
};

FString Regular_PlayCardOrMoveUnit(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
FString Regular_WhereToPlayCard(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);

FString Regular_PlayCardOrMoveUnit(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    UE_LOG(LogTemp, Error, TEXT("Regular_PlayCardOrMoveUnit")); 
    if (cbt == FightingUIManagerClickType::OnCard) {
        card->PermanentlyHighlight();
        uim->state["card"] = card;
        uim->Clear()->RegisterCallback(Regular_WhereToPlayCard, all_click_types)->WaitForInput();
        return "";
    }
    return "";
}

FString Regular_WhereToPlayCard(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    UE_LOG(LogTemp, Error, TEXT("Regular_WhereToPlayCard"));
    if (cbt == FightingUIManagerClickType::OnCell) {
        ABActorFightingCard* ChosenCard = std::any_cast<ABActorFightingCard*>(uim->state["card"]);
        ChosenCard->ResetPermanentHighlight();
        uim->Clear()->RegisterCallback(Regular_PlayCardOrMoveUnit, all_click_types)->WaitForInput();
        return "";
    }
    return "";
}





void FightingUIManager::LetActionsRegular() {
    Clear()->RegisterCallback(Regular_PlayCardOrMoveUnit, all_click_types)->WaitForInput();
}