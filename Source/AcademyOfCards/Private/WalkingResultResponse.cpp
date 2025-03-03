// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultResponse.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultResponse::WalkingResultResponse(TSharedPtr<FJsonObject> data)
{
	if (data->HasField("text")) {
		Response = data->GetStringField("text");
	}
	else
	{
		Response = "...";
	}
}

WalkingResultResponse::~WalkingResultResponse()
{
}

void WalkingResultResponse::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	walking_event->TextFromResult += Response + "\n";
}