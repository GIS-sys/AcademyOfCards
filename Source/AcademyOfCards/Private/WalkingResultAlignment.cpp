// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultAlignment.h"

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
