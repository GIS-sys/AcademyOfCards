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
class FightingUIManager;

enum FightingUIManagerClickType {
	OnCell = 0,
	OnUnit,
	OnAbility,
	OnCard,
	OnPassTurn,
};

using CallbackType = std::function<FString(FightingUIManagerClickType, FightingUIManager*, ABActorFightingCellBase*, ABActorFightingUnitBase*, TriggersDispatcherEvent_EnumAbility, ABActorFightingCard*)>;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingUIManager
{
	ABActorFightingField* Field = nullptr;
	std::map<FightingUIManagerClickType, CallbackType> Callbacks;
	bool wait = false;

public:
	std::map<FString, std::any> state;

	FightingUIManager();
	~FightingUIManager();

	void Init(ABActorFightingField* NewField);

	void LetActionsRegular();

	bool IsWaitingPlayerResponse() const;

	FString ClickedOnCell(ABActorFightingCellBase* target);
	FString ClickedOnUnit(ABActorFightingUnitBase* target);
	FString ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target);
	FString ClickedOnCard(ABActorFightingCard* target);
	FString ClickedOnPassTurn();

	void WaitForInput();
	FightingUIManager* Clear();

	FightingUIManager* RegisterCallback(CallbackType callback_foo, std::vector<FightingUIManagerClickType> click_types);
};
