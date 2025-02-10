// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingEvent.h"
#include <WalkingOption.h>

WalkingEvent::WalkingEvent(FString name, TSharedPtr<FJsonObject> data)
{
	this->Name = name;
	this->Text = data->TryGetField("text")->AsString();
	for (auto& option : data->GetArrayField("options")) {
		this->Options.Add(MakeShareable(new WalkingOption(option->AsObject())));
	}
}

WalkingEvent::~WalkingEvent()
{
}
