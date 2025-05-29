// Fill out your copyright notice in the Description page of Project Settings.


#include "Fighting/FightingUIManager.h"
#include "BActorFightingField.h"

using FCT = FightingUIManagerClickType;

std::vector<FCT> FightingUIManagerClickType_All() {
    return { OnCell, OnUnit, OnAbility, OnCard, OnPassTurn, OnOutside };
}

std::vector<FCT> FightingUIManagerClickType_AllExcept(const std::set<FCT>& exceptions) {
    std::vector<FCT> result;
    for (FCT ct : FightingUIManagerClickType_All())
        if (exceptions.find(ct) == exceptions.end())
            result.push_back(ct);
    return result;
}


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
    if (Callbacks.find(FCT::OnCell) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnCell](FCT::OnCell, this, target, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnUnit(ABActorFightingUnitBase* target) {
    if (Callbacks.find(FCT::OnUnit) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnUnit](FCT::OnUnit, this, nullptr, target, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target) {
    if (Callbacks.find(FCT::OnAbility) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnAbility](FCT::OnAbility, this, nullptr, nullptr, target, nullptr);
}

FString FightingUIManager::ClickedOnCard(ABActorFightingCard* target) {
    if (Callbacks.find(FCT::OnCard) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnCard](FCT::OnCard, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, target);
}

FString FightingUIManager::ClickedOnPassTurn() {
    if (Callbacks.find(FCT::OnPassTurn) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnPassTurn](FCT::OnPassTurn, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FString FightingUIManager::ClickedOnOutside() {
    if (Callbacks.find(FCT::OnOutside) == Callbacks.end()) return "Press on something else";
    return Callbacks[FCT::OnOutside](FCT::OnOutside, this, nullptr, nullptr, TriggersDispatcherEvent_EnumAbility::None, nullptr);
}

FightingUIManager* FightingUIManager::RegisterCallback(CallbackType callback_foo, std::vector<FCT> click_types) {
    for (auto cbt : click_types) {
        Callbacks[cbt] = callback_foo;
    }
    return this;
}

FightingUIManager* FightingUIManager::FillUnusedCallbacks(CallbackType callback_foo) {
    for (FCT cbt : FightingUIManagerClickType_All()) {
        if (Callbacks.find(cbt) == Callbacks.end())
            Callbacks[cbt] = callback_foo;
    }
    return this;
}



FString Regular_Pass(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
FString Regular_Abilities(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
FString Regular_Outside(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);

FString Regular_PlayCardOrMoveUnit(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
std::vector<FCT> Regular_PlayCardOrMoveUnit_CallBackTypes {
    FCT::OnCard,
    FCT::OnUnit,
};

FString Regular_WhereToPlayCard(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
std::vector<FCT> Regular_WhereToPlayCard_CallBackTypes {
    FCT::OnCell,
    FCT::OnUnit,
    FCT::OnCard,
};
FString Regular_WhereToMoveUnit(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card);
std::vector<FCT> Regular_WhereToMoveUnit_CallBackTypes {
    FCT::OnCard,
    FCT::OnUnit
};


FString Regular_Pass(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    Regular_Outside(cbt, uim, cell, unit, ability, card);
    return uim->Field->PassTurnWithEvent();
}

FString Regular_Outside(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    if (uim->state.find("unit") != uim->state.end())
        std::any_cast<ABActorFightingUnitBase*>(uim->state["unit"])->ResetPermanentHighlight();
    if (uim->state.find("card") != uim->state.end())
        std::any_cast<ABActorFightingCard*>(uim->state["card"])->ResetPermanentHighlight();

    uim->LetActionsRegular();
    return "";
}

FString Regular_Abilities(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    if (ability == TriggersDispatcherEvent_EnumAbility::DrawCard) {
        return uim->Field->AbilityDrawCardWithEvent();
    }
    if (ability == TriggersDispatcherEvent_EnumAbility::GetManaDark) {
        return uim->Field->AbilityGetManaDarkWithEvent();
    }
    if (ability == TriggersDispatcherEvent_EnumAbility::GetManaLight) {
        return uim->Field->AbilityGetManaLightWithEvent();
    }
    if (ability == TriggersDispatcherEvent_EnumAbility::GetManaIce) {
        return uim->Field->AbilityGetManaIceWithEvent();
    }
    if (ability == TriggersDispatcherEvent_EnumAbility::GetManaFire) {
        return uim->Field->AbilityGetManaFireWithEvent();
    }
    return "Unknown ability";
}

FString Regular_PlayCardOrMoveUnit(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    if (cbt == FCT::OnCard) {
        card->PermanentlyHighlight();
        uim->state["card"] = card;
        uim->Clear()
            ->RegisterCallback(Regular_WhereToPlayCard, Regular_WhereToPlayCard_CallBackTypes)
            ->RegisterCallback(Regular_Pass, { FCT::OnPassTurn })
            ->RegisterCallback(Regular_Outside, { FCT::OnOutside })
            ->RegisterCallback(Regular_Abilities, { FCT::OnAbility })
            ->WaitForInput();
        return "";
    }
    if (cbt == FCT::OnUnit) {
        unit->PermanentlyHighlight();
        uim->state["unit"] = unit;
        uim->Clear()
            ->RegisterCallback(Regular_WhereToMoveUnit, Regular_WhereToMoveUnit_CallBackTypes)
            ->RegisterCallback(Regular_Pass, { FCT::OnPassTurn })
            ->RegisterCallback(Regular_Outside, { FCT::OnOutside })
            ->RegisterCallback(Regular_Abilities, { FCT::OnAbility })
            ->WaitForInput();
        return "";
    }
    return "Choose Card, Unit, Ability or Pass the turn";
}

FString Regular_WhereToPlayCard(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    ABActorFightingCard* ChosenCard = std::any_cast<ABActorFightingCard*>(uim->state["card"]);
    if (cbt == FCT::OnCell) {
        if (!ChosenCard->CanTargetCell()) return "Please target Cell with this card";
        uim->Field->PlayCardWithEvent(ChosenCard, cell);
        ChosenCard->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FCT::OnUnit) {
        if (!ChosenCard->CanTargetUnit()) return "Please target Unit with this card";
        uim->Field->PlayCardWithEvent(ChosenCard, unit->CurrentCell);
        ChosenCard->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FCT::OnCard) {
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
    if (cbt == FCT::OnPassTurn) {
        return "Please explicitly deselect current Card first";
    }
    return "";
}

FString Regular_WhereToMoveUnit(FCT cbt, FightingUIManager* uim, ABActorFightingCellBase* cell, ABActorFightingUnitBase* unit, TriggersDispatcherEvent_EnumAbility ability, ABActorFightingCard* card) {
    ABActorFightingUnitBase* ChosenUnit = std::any_cast<ABActorFightingUnitBase*>(uim->state["unit"]);
    if (cbt == FCT::OnCell) {
        uim->Field->MoveUnitWithEvent(ChosenUnit, cell);
        ChosenUnit->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    if (cbt == FCT::OnUnit) {
        if (unit == ChosenUnit) {
            ChosenUnit->ResetPermanentHighlight();
            uim->LetActionsRegular();
            return "";
        }
        uim->Field->AttackUnitWithEvent(ChosenUnit, unit);
        ChosenUnit->ResetPermanentHighlight();
        uim->LetActionsRegular();
        return "";
    }
    return "";
}






void FightingUIManager::LetActionsRegular() {
    Clear()
        ->RegisterCallback(Regular_PlayCardOrMoveUnit, Regular_PlayCardOrMoveUnit_CallBackTypes)
        ->RegisterCallback(Regular_Pass, { FCT::OnPassTurn })
        ->RegisterCallback(Regular_Outside, { FCT::OnOutside })
        ->RegisterCallback(Regular_Abilities, { FCT::OnAbility })
        ->WaitForInput();
}