// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ABActorFightingField;
class ABActorFightingCellBase;
class ABActorFightingUnitBase;
class ABActorFightingCard;

/**
 * 
 */
class ACADEMYOFCARDS_API FightingUIManager
{
	ABActorFightingField* Field = nullptr;

public:
	FightingUIManager();
	~FightingUIManager();

	void Init(ABActorFightingField* NewField);

	void LetActionsRegular();

	bool IsWaitingPlayerResponse() const;

	bool ClickedOnCell(ABActorFightingCellBase* target);
	bool ClickedOnUnit(ABActorFightingUnitBase* target);
	bool ClickedOnAbility(FString target);
	bool ClickedOnCard(ABActorFightingCard* target);
	bool ClickedOnPassTurn();
};
