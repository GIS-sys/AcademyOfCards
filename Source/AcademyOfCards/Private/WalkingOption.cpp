// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingOption.h"
#include <WalkingResult.h>

WalkingOption::WalkingOption(TSharedPtr<FJsonObject> data)
{
	Text = data->TryGetField("text")->AsString();
	for (auto& [x, y] : data->GetObjectField("result")->Values) {
		Results.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
	}
}

WalkingOption::~WalkingOption()
{
}
