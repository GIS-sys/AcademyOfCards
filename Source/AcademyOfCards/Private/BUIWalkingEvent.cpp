// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIWalkingEvent.h"

void UBUIWalkingEvent::NewEventPopup_Start()
{

}

void UBUIWalkingEvent::NewEventPopup_Finish()
{

}

void UBUIWalkingEvent::NewEventPopup_SetText(FString Text)
{

}

void UBUIWalkingEvent::NewEventPopup_AddButton(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults)
{
	UE_LOG(LogTemp, Log, TEXT("CreateButton ButtonName: %s %d"), *ButtonName, ButtonResults.Num());
}
