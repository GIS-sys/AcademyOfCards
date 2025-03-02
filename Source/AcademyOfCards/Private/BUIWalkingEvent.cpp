// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include <WalkingOption.h>

void UBUIWalkingEvent::NewEventPopup_Clear(bool ForgetChosenCard)
{
    EventIsShown = false;
    if (CurrentWalkingCard && ForgetChosenCard) {
        CurrentWalkingCard->IsCloseUpLook = false;
        CurrentWalkingCard = nullptr;
    }
    if (EventPopupVerticalBox) EventPopupVerticalBox->ClearChildren();
    if (EventPopupLabel) EventPopupLabel->SetText(FText::FromString(""));
}

void UBUIWalkingEvent::NewEventPopup_Finish(ABActorWalkingCard* WalkingCard)
{
    EventIsShown = true;
    CurrentWalkingCard = WalkingCard;
    CurrentWalkingCard->IsCloseUpLook = true;
    if (!EventPopupVerticalBox->HasAnyChildren()) {
        TSharedPtr<WalkingOption> Option = WalkingOption::FactoryCreateCloseOption();
        NewEventPopup_AddButton(Option->Text, Option->Results);
    }
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
    TextFromResult = "";
    CloseFromResult = false;
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
    ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);
    for (const auto& Result : ButtonResults) {
        Result->Execute(this, PlayerModel);
    }
    if (CloseFromResult) {
        NewEventPopup_Clear();
        return;
    }
    NewEventPopup_Clear(false);
    NewEventPopup_SetText(TextFromResult); // TODO
    NewEventPopup_Finish(CurrentWalkingCard);
}