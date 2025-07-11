// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fighting/FightingTriggersDispatcher.h"
#include <functional>
#include <map>
#include <set>
#include <vector>

class ABActorFightingField;
class ABActorFightingCellBase;
class ABActorFightingUnitBase;
class ABActorFightingCard;
class FightingUIManager;

enum FightingUIManagerClickType {
	OnCell = 0,
	OnUnit,
	OnAbility,
	OnCard,
	OnPassTurn,
	OnOutside,
};

std::vector<FightingUIManagerClickType> FightingUIManagerClickType_All();
std::vector<FightingUIManagerClickType> FightingUIManagerClickType_AllExcept(const std::set<FightingUIManagerClickType>& exceptions);

using CallbackType = std::function<FString(FightingUIManagerClickType, FightingUIManager*, ABActorFightingCellBase*, ABActorFightingUnitBase*, TriggersDispatcherEvent_EnumAbility, ABActorFightingCard*)>;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingUIManager
{
	std::map<FightingUIManagerClickType, CallbackType> Callbacks;
	bool trigger_needs_input = false;

	bool CheckCanClickNow(bool is_ai) const;

public:
	std::map<FString, std::any> state;
	ABActorFightingField* Field = nullptr;

	FightingUIManager();
	~FightingUIManager();

	void Init(ABActorFightingField* NewField);

	void LetActionsRegular();

	bool IsTriggerWaitingPlayerResponse() const;
	void TriggerNeedsInput();
	void TriggerDoesntNeedInput();

	bool CheckIfCallbackExists(FightingUIManagerClickType click_type) const;


	FString ClickedOnCell(ABActorFightingCellBase* target, bool is_ai = false);
	FString ClickedOnUnit(ABActorFightingUnitBase* target, bool is_ai = false);
	FString ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target, bool is_ai = false);
	FString ClickedOnCard(ABActorFightingCard* target, bool is_ai = false);
	FString ClickedOnPassTurn(bool is_ai = false);
	FString ClickedOnOutside(bool is_ai = false);

	FightingUIManager* Clear();

	FightingUIManager* RegisterCallback(CallbackType callback_foo, std::vector<FightingUIManagerClickType> click_types);
	FightingUIManager* FillUnusedCallbacks(CallbackType callback_foo);
};
