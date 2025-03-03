// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingOption.h"
#include <WalkingResult.h>

WalkingOption::WalkingOption(FString text) : Text(text) {}

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

TSharedPtr<WalkingOption> WalkingOption::FactoryCreateCloseOption() {
	TSharedPtr<WalkingOption> CloseOption = MakeShareable(new WalkingOption("Close"));
	CloseOption->Results.Add(WalkingResult::FactoryCreate("__close__", MakeShareable(new FJsonObject())));
	return CloseOption;
}