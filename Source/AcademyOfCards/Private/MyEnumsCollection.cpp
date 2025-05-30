// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnumsCollection.h"

FString TriggersDispatcherEvent_EnumAbility_ToString(TriggersDispatcherEvent_EnumAbility ev) {
	if (ev == TriggersDispatcherEvent_EnumAbility::DRAW_CARD) return "DRAW_CARD";
	if (ev == TriggersDispatcherEvent_EnumAbility::DREW_CARD) return "DREW_CARD";
	if (ev == TriggersDispatcherEvent_EnumAbility::PASS_TURN) return "PASS_TURN";
	if (ev == TriggersDispatcherEvent_EnumAbility::PASSED_TURN) return "PASSED_TURN";
	if (ev == TriggersDispatcherEvent_EnumAbility::GET_MANA_LIGHT) return "GET_MANA_LIGHT";
	if (ev == TriggersDispatcherEvent_EnumAbility::GOT_MANA_LIGHT) return "GOT_MANA_LIGHT";
	if (ev == TriggersDispatcherEvent_EnumAbility::GET_MANA_DARK) return "GET_MANA_DARK";
	if (ev == TriggersDispatcherEvent_EnumAbility::GOT_MANA_DARK) return "GOT_MANA_DARK";
	if (ev == TriggersDispatcherEvent_EnumAbility::GET_MANA_FIRE) return "GET_MANA_FIRE";
	if (ev == TriggersDispatcherEvent_EnumAbility::GOT_MANA_FIRE) return "GOT_MANA_FIRE";
	if (ev == TriggersDispatcherEvent_EnumAbility::GET_MANA_ICE) return "GET_MANA_ICE";
	if (ev == TriggersDispatcherEvent_EnumAbility::GOT_MANA_ICE) return "GOT_MANA_ICE";
	return "UNK";
}

FString TriggersDispatcherEvent_EnumEvent_ToString(TriggersDispatcherEvent_EnumEvent ev) {
	if (ev == TriggersDispatcherEvent_EnumEvent::MOVE) return "MOVE";
	if (ev == TriggersDispatcherEvent_EnumEvent::MOVED) return "MOVED";
	if (ev == TriggersDispatcherEvent_EnumEvent::ATTACK) return "ATTACK";
	if (ev == TriggersDispatcherEvent_EnumEvent::ATTACKED) return "ATTACKED";
	if (ev == TriggersDispatcherEvent_EnumEvent::PLAY_CARD) return "PLAY_CARD";
	if (ev == TriggersDispatcherEvent_EnumEvent::PLAYED_CARD) return "PLAYED_CARD";
	if (ev == TriggersDispatcherEvent_EnumEvent::DRAW_CARD_ON_TURN_START) return "DRAW_CARD_ON_TURN_START";
	if (ev == TriggersDispatcherEvent_EnumEvent::DREW_CARD_ON_TURN_START) return "DREW_CARD_ON_TURN_START";
	if (ev == TriggersDispatcherEvent_EnumEvent::TAKE_DAMAGE) return "TAKE_DAMAGE";
	if (ev == TriggersDispatcherEvent_EnumEvent::TOOK_DAMAGE) return "TOOK_DAMAGE";
	if (ev == TriggersDispatcherEvent_EnumEvent::UNIT_DYING) return "UNIT_DYING";
	if (ev == TriggersDispatcherEvent_EnumEvent::UNIT_DIED) return "UNIT_DIED";
	return "UNK";
}


UMyEnumsCollection::UMyEnumsCollection()
{
}

UMyEnumsCollection::~UMyEnumsCollection()
{
}
