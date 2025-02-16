// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WalkingResult.h"
#include "BUIWalkingEvent.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API UBUIWalkingEvent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NewEventPopup_Start();
	void NewEventPopup_Finish();
	void NewEventPopup_SetText(FString Text);
	void NewEventPopup_AddButton(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults);
};
