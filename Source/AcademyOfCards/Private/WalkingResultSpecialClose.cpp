// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultSpecialClose.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultSpecialClose::WalkingResultSpecialClose(TSharedPtr<FJsonObject> data)
{
}

WalkingResultSpecialClose::~WalkingResultSpecialClose()
{
}

void WalkingResultSpecialClose::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	walking_event->CloseFromResult = true;
}