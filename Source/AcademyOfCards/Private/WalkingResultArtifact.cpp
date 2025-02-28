// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultArtifact.h"

WalkingResultArtifact::WalkingResultArtifact(TSharedPtr<FJsonObject> data)
{
	ID = data->GetStringField("id");
}

WalkingResultArtifact::~WalkingResultArtifact()
{
}
