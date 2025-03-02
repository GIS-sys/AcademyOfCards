// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultArtifact.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultArtifact::WalkingResultArtifact(TSharedPtr<FJsonObject> data)
{
	ID = data->GetStringField("id");
}

WalkingResultArtifact::~WalkingResultArtifact()
{
}

void WalkingResultArtifact::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	walking_event->TextFromResult += "Artifact: " + ID + "\n"; // TOOD add real artifacts to player
}
