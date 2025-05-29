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
    if (Callbacks.find(FightingUIManagerClickType::OnCell) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnCell](FightingUIManagerClickType::OnCell, this, target, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnUnit(ABActorFightingUnitBase* target) {
    if (Callbacks.find(FightingUIManagerClickType::OnUnit) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnUnit](FightingUIManagerClickType::OnUnit, this, nullptr, target, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target) {
    if (Callbacks.find(FightingUIManagerClickType::OnAbility) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnAbility](FightingUIManagerClickType::OnAbility, this, nullptr, nullptr, target, nullptr);
}

FString FightingUIManager::ClickedOnCard(ABActorFightingCard* target) {
    if (Callbacks.find(FightingUIManagerClickType::OnCard) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnCard](FightingUIManagerClickType::OnCard, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, target);
}

FString FightingUIManager::ClickedOnPassTurn() {
    if (Callbacks.find(FightingUIManagerClickType::OnPassTurn) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnPassTurn](FightingUIManagerClickType::OnPassTurn, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnOutside() {
    if (Callbacks.find(FightingUIManagerClickType::OnOutside) == Callbacks.end()) return "Press on something else";
    return Callbacks[FightingUIManagerClickType::OnOutside](FightingUIManagerClickType::OnOutside, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
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
    FightingUIManagerClickType::OnPassTurn,
    FightingUIManagerClickType::OnOutside,
};

FString Regular_PlayCardOrMoveUnitOrPass(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
FString Regular_WhereToPlayCard(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
FString Regular_WhereToMoveUnit(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);


FString Regular_PlayCardOrMoveUnitOrPass(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    if (cbt == FightingUIManagerClickType::OnCard) {
        card->PermanentlyHighlight();
        uim->state["card"] = card;
        uim->Clear()->RegisterCallback(Regular_WhereToPlayCard, all_click_types)->WaitForInput();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnUnit) {
        unit->PermanentlyHighlight();
        uim->state["unit"] = unit;
        uim->Clear()->RegisterCallback(Regular_WhereToMoveUnit, all_click_types)->WaitForInput();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnPassTurn) {
        uim->Clear();
        uim->Field->PassTurn();
        return "";
    }
    return "";
}

FString Regular_WhereToPlayCard(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    ABActorFightingCard* ChosenCard = std::any_cast<ABActorFightingCard*>(uim->state["card"]);
    if (cbt == FightingUIManagerClickType::OnCell) {
        if (!ChosenCard->CanTargetCell()) return "Please target Cell with this card";
        bool res = uim->Field->PlayCard(ChosenCard, cell);
        if (!res) return "Couldn't play Card on this Cell";
        ChosenCard->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnUnit) {
        if (!ChosenCard->CanTargetUnit()) return "Please target Unit with this card";
        bool res = uim->Field->PlayCard(ChosenCard, unit->CurrentCell);
        if (!res) return "Couldn't play Card on this Unit";
        ChosenCard->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnOutside) {
        ChosenCard->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnCard) {
        // Dehighlight current
        ChosenCard->ResetPermanentHighlight();
        if (ChosenCard != card) {
            card->PermanentlyHighlight();
            uim->state["card"] = card;
        } else {
            uim->LetActionsRegular();
        }
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnPassTurn) {
        return "Please explicitly deselect current Card first";
    }
    return "";
}

FString Regular_WhereToMoveUnit(FightingUIManagerClickType cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    ABActorFightingUnitBase* ChosenUnit = std::any_cast<ABActorFightingUnitBase*>(uim->state["unit"]);
    if (cbt == FightingUIManagerClickType::OnCell) {
        bool res = uim->Field->MoveUnit(ChosenUnit, cell);
        if (!res) return "Couldn't move Unit on this Cell";
        ChosenUnit->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnUnit) {
        if (unit == ChosenUnit) {
            ChosenUnit->ResetPermanentHighlight();
            uim->LetActionsRegular();
            return "";
        }
        bool res = uim->Field->AttackUnit(ChosenUnit, unit);
        if (!res) return "Couldn't attack Unit with selected Unit";
        ChosenUnit->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnOutside) {
        ChosenUnit->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FightingUIManagerClickType::OnPassTurn) {
        return "Please explicitly deselect current Unit first";
    }
    return "";
}





void FightingUIManager::LetActionsRegular() {
    Clear()->RegisterCallback(Regular_PlayCardOrMoveUnitOrPass, all_click_types)->WaitForInput();
}