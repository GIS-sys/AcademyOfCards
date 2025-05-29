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
	ATTACKED,
	PLAY_CARD,
	//DRAW_CARD,
	DRAW_CARD_ON_TURN_START,
	DREW_CARD_ON_TURN_START,
	TAKE_DAMAGE,
	TOOK_DAMAGE,
	UNIT_DYING,
	UNIT_DIED,
};

FString TriggersDispatcherEvent_EnumEvent_ToString(TriggersDispatcherEvent_EnumEvent ev) {
	// TODO IMPORTANT
	if (ev == TriggersDispatcherEvent_EnumEvent::MOVE) return "MOVE";
	if (ev == TriggersDispatcherEvent_EnumEvent::ATTACK) return "ATTACK";
	if (ev == TriggersDispatcherEvent_EnumEvent::ATTACKED) return "ATTACKED";
	if (ev == TriggersDispatcherEvent_EnumEvent::PLAY_CARD) return "PLAY_CARD";
	//if (ev == TriggersDispatcherEvent_EnumEvent::DRAW_CARD) return "DRAW_CARD";
	if (ev == TriggersDispatcherEvent_EnumEvent::DRAW_CARD_ON_TURN_START) return "DRAW_CARD_ON_TURN_START";
	if (ev == TriggersDispatcherEvent_EnumEvent::DREW_CARD_ON_TURN_START) return "DREW_CARD_ON_TURN_START";
	if (ev == TriggersDispatcherEvent_EnumEvent::TAKE_DAMAGE) return "TAKE_DAMAGE";
	if (ev == TriggersDispatcherEvent_EnumEvent::TOOK_DAMAGE) return "TOOK_DAMAGE";
	if (ev == TriggersDispatcherEvent_EnumEvent::UNIT_DYING) return "UNIT_DYING";
	if (ev == TriggersDispatcherEvent_EnumEvent::UNIT_DIED) return "UNIT_DIED";
	return "UNK";
}

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
