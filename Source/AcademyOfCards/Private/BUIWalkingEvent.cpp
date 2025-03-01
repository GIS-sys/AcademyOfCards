// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIWalkingEvent.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UBUIWalkingEvent::NewEventPopup_Clear()
{
    EventIsShown = false;
    if (EventPopupVerticalBox) EventPopupVerticalBox->ClearChildren();
    if (EventPopupLabel) EventPopupLabel->SetText(FText::FromString(""));
}

void UBUIWalkingEvent::NewEventPopup_Finish()
{
    EventIsShown = true;
}

void UBUIWalkingEvent::NewEventPopup_SetText(FString Text)
{
    if (!EventPopupLabel) return;

    EventPopupLabel->SetText(FText::FromString(Text));
}

void UBUIWalkingEvent::NewEventPopup_AddButton(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults)
{
    if (!EventPopupVerticalBox) return;

    // create button
    UButton* NewButton = NewObject<UButton>(this);
    // create label
    UTextBlock* ButtonText = NewObject<UTextBlock>(NewButton);
    ButtonText->SetText(FText::FromString(ButtonName));
    ButtonText->SetAutoWrapText(true);
    NewButton->AddChild(ButtonText);
    // add onclick
    SButton* ButtonWidget = (SButton*)&(NewButton->TakeWidget().Get());
    ButtonWidget->SetOnClicked(FOnClicked::CreateLambda([this, ButtonName, ButtonResults]()
        {
            EventPopupButtonOnClicked(ButtonName, ButtonResults);
            return FReply::Handled();
        }
    ));
    // add to box
    EventPopupVerticalBox->AddChild(NewButton);
}

void UBUIWalkingEvent::EventPopupButtonOnClicked(FString ButtonName, TArray<TSharedPtr<WalkingResult>> ButtonResults)
{
    for (const auto& Result : ButtonResults) {
        Result->Execute(); // TODO actually execute result after button is clicked
    }
    NewEventPopup_Clear();
}