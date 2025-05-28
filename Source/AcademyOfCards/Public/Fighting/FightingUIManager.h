// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fighting/FightingTriggersDispatcher.h"
#include <functional>
#include <map>

class ABActorFightingField;
class ABActorFightingCellBase;
class ABActorFightingUnitBase;
class ABActorFightingCard;

enum FightingUIManagerClickType {
	OnCell = 0,
	OnUnit,
	OnAbility,
	OnCard,
	OnPassTurn,
};

using CallbackType = std::function<bool(FightingUIManagerClickType, ABActorFightingCellBase*, ABActorFightingUnitBase*, TriggersDispatcherEvent_EnumAbility, ABActorFightingCard*)>;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingUIManager
{
	ABActorFightingField* Field = nullptr;
	std::map<FightingUIManagerClickType, CallbackType> Callbacks;
	bool wait = false;

public:
	FightingUIManager();
	~FightingUIManager();

	void Init(ABActorFightingField* NewField);

	void LetActionsRegular();

	bool IsWaitingPlayerResponse() const;

	bool ClickedOnCell(ABActorFightingCellBase* target);
	bool ClickedOnUnit(ABActorFightingUnitBase* target);
	bool ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target);
	bool ClickedOnCard(ABActorFightingCard* target);
	bool ClickedOnPassTurn();

	void WaitForInput();
	void Clear();

	void RegisterCallback(CallbackType callback_foo, std::vector<FightingUIManagerClickType> callback_types);
};
