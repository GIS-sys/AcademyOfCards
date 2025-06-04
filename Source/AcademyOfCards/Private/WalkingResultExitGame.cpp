 // Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultExitGame.h"
#include <Kismet/KismetSystemLibrary.h>
#include "BActorWalkingPlayerModel.h"

WalkingResultExitGame::WalkingResultExitGame(TSharedPtr<FJsonObject> data)
{
}

WalkingResultExitGame::~WalkingResultExitGame()
{
}

void WalkingResultExitGame::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model) {
    UKismetSystemLibrary::QuitGame(player_model->GetWorld(), nullptr, EQuitPreference::Quit, true);
}