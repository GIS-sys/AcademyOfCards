// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WalkingResult.h"
#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>
#include "BActorWalkingCard.h"
#include <CustomButtonDefault.h>
#include "BUIWalkingEvent.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API UBUIWalkingEvent : public UUserWidget
{
	GENERATED_BODY()
	
	bool IsAddCloseButtonAuto = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Widgets")
	UVerticalBox* EventPopupVerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup Widgets")
	UTextBlock* EventPopupLabel;

public:
	void NewEventPopup_Clear(bool ForgetChosenCard = true);
	void NewEventPopup_Finish(ABActorWalkingCard* WalkingCard);
	void NewEventPopup_SetText(FString Text);
	void NewEventPopup_AddButton(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults);

	void DontAddCloseButtonOnce() { IsAddCloseButtonAuto = false; }

	void EventPopupButtonOnClicked(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsShown")
	bool EventIsShown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IsShown")
	ABActorWalkingCard* CurrentWalkingCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	TSubclassOf<UCustomButtonDefault> ButtonWidget;

	FString TextFromResult;
	bool CloseFromResult = false;
	TArray<std::pair<FString, TArray<TSharedPtr<WalkingResult>>>> ButtonsFromResult;
};
