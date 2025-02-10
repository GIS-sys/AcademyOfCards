// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultAlignment.h"

WalkingResultAlignment::WalkingResultAlignment(TSharedPtr<FJsonObject> data)
{
	if (data->HasField("dark"))
	{
		DarkAlignment = data->GetNumberField("dark");
	}
}

WalkingResultAlignment::~WalkingResultAlignment()
{
}
