// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumsCollection.generated.h"

UENUM(BlueprintType)
enum class TriggersDispatcherEvent_EnumAbility : uint8 {
	None = 0,
	DrawCard,
	PassTurn,
	GetManaLight,
	GetManaDark,
	GetManaFire,
	GetManaIce
};

UENUM(BlueprintType)
enum class TriggersDispatcherEvent_EnumEvent : uint8 {
	MOVE = 0,
	ATTACK,
	PLAYCARD,
	DRAWCARD,
	DrawCardOnTurnStart
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ACADEMYOFCARDS_API UMyEnumsCollection : public UObject
{
	GENERATED_BODY()

public:
	UMyEnumsCollection();
	~UMyEnumsCollection();
};
