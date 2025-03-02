// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultAlignment.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"

WalkingResultAlignment::WalkingResultAlignment(TSharedPtr<FJsonObject> data)
{
	if (data->HasField("dark"))
	{
		DarkAlignment = data->GetNumberField("dark");
	}
	if (data->HasField("light"))
	{
		LightAlignment = data->GetNumberField("light");
	}
	if (data->HasField("ice"))
	{
		IceAlignment = data->GetNumberField("ice");
	}
	if (data->HasField("fire"))
	{
		FireAlignment = data->GetNumberField("fire");
	}
}

WalkingResultAlignment::~WalkingResultAlignment()
{
}

void WalkingResultAlignment::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	if (DarkAlignment != 0) {
		player_model->PlayerStats.AlignmentDark += DarkAlignment;
		walking_event->TextFromResult += "Dark Alignment: " + FString::SanitizeFloat(DarkAlignment) + "\n";
	}
	if (LightAlignment != 0) {
		player_model->PlayerStats.AlignmentLight += LightAlignment;
		walking_event->TextFromResult += "Light Alignment: " + FString::SanitizeFloat(LightAlignment) + "\n";
	}
	if (IceAlignment != 0) {
		player_model->PlayerStats.AlignmentIce += IceAlignment;
		walking_event->TextFromResult += "Ice Alignment: " + FString::SanitizeFloat(IceAlignment) + "\n";
	}
	if (FireAlignment != 0) {
		player_model->PlayerStats.AlignmentFire += FireAlignment;
		walking_event->TextFromResult += "Fire Alignment: " + FString::SanitizeFloat(FireAlignment) + "\n";
	}
}