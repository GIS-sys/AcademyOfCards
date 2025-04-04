// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingEvent.h"
#include <WalkingOption.h>
#include <BUIWalkingPlayerStats.h>
#include "BActorWalkingDealer.h"
#include "BUIHUD.h"
#include "BUIWalkingEvent.h"
#include "BWalking_UI.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

WalkingEvent::WalkingEvent(FString id, FString name, FString text, TArray<TSharedPtr<WalkingOption>> options)
{
	ID = id;
	Name = name;
	Text = text;
	Options = options;
}

WalkingEvent::WalkingEvent(FString id, TSharedPtr<FJsonObject> data)
{
	ID = id;
	Name = data->TryGetField("name")->AsString();
	Text = data->TryGetField("text")->AsString();
	for (auto& option : data->GetArrayField("options")) {
		Options.Add(MakeShareable(new WalkingOption(option->AsObject())));
	}
}

WalkingEvent::~WalkingEvent()
{
}

void WalkingEvent::Fire(ABActorWalkingDealer* DealerPtr, ABActorWalkingCard* WalkingCard)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(DealerPtr->GetWorld(), 0);
	if (!PlayerController) return;
    ABUIHUD* HUD = Cast<ABUIHUD>(PlayerController->GetHUD());
	if (!HUD) return;
	TObjectPtr<UBWalking_UI> MainMenu = Cast<UBWalking_UI>(HUD->MainMenu);
	UBUIWalkingEvent* EventUI = MainMenu->BUIWalkingEvent;

	EventUI->NewEventPopup_Clear();
	EventUI->NewEventPopup_SetText(Text);
	for (auto& OptionPtr : Options) {
		FString ButtonName = OptionPtr->Text;
		TArray<TSharedPtr<WalkingResult>> ButtonResults = OptionPtr->Results;
		EventUI->NewEventPopup_AddButton(ButtonName, ButtonResults);
	}
	EventUI->NewEventPopup_Finish(WalkingCard);
}

TSharedPtr<WalkingEvent> WalkingEvent::CreateDefault()
{
	return MakeShareable(new WalkingEvent("", "", "Nothing happened", TArray<TSharedPtr<WalkingOption>>()));
}