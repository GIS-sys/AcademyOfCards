// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnumsCollection.generated.h"

UENUM(BlueprintType)
enum class TriggersDispatcherEvent_EnumAbility : uint8 {
	UNK = 0,
	NONE,
	DRAW_CARD,
	DREW_CARD,
	PASS_TURN,
	PASSED_TURN,
	GET_MANA_LIGHT,
	GOT_MANA_LIGHT,
	GET_MANA_DARK,
	GOT_MANA_DARK,
	GET_MANA_FIRE,
	GOT_MANA_FIRE,
	GET_MANA_ICE,
	GOT_MANA_ICE,
};

UENUM(BlueprintType)
enum class TriggersDispatcherEvent_EnumEvent : uint8 {
	UNK = 0,
	MOVE,
	MOVED,
	ATTACK,
	ATTACKED,
	PLAY_CARD,
	PLAYED_CARD,
	DRAW_CARD_ON_TURN_START,
	DREW_CARD_ON_TURN_START,
	TAKE_DAMAGE,
	TOOK_DAMAGE,
	UNIT_DYING,
	UNIT_DIED,
};

FString TriggersDispatcherEvent_EnumAbility_ToString(TriggersDispatcherEvent_EnumAbility ev);
FString TriggersDispatcherEvent_EnumEvent_ToString(TriggersDispatcherEvent_EnumEvent ev);

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
