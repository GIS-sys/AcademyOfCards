// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultFight.h"
#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"
#include "WalkingOption.h"
#include <Kismet/GameplayStatics.h>
#include <BUIGameModeBase.h>

WalkingResultFight::WalkingResultFight(TSharedPtr<FJsonObject> data)
{
	Opponent = data->GetStringField("who");
	for (auto& [x, y] : data->GetObjectField("reward_win")->Values) {
		ResultsWin.Add(WalkingResult::FactoryCreate(x, y->AsObject()));
	}
}

WalkingResultFight::~WalkingResultFight()
{
}

void WalkingResultFight::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model)
{
	FWorldContext* world = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
	ABUIGameModeBase* MyMode = Cast<ABUIGameModeBase>(UGameplayStatics::GetGameMode(world->World()));
	walking_event->DontAddCloseButtonOnce();
	MyMode->SwitchToFight(this);
	//WalkingOption::FactoryCreateCloseOption()->Results[0]->Execute(walking_event, player_model); // TODO
}

void WalkingResultFight::ExecuteAfterFight(UBUIWalkingEvent* walking_event, ABActorWalkingCard* current_card)
{
	walking_event->CurrentWalkingCard = current_card;
	walking_event->EventPopupButtonOnClicked("", ResultsWin);
}