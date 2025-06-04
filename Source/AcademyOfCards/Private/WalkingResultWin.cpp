// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingResultWin.h"

#include "BUIWalkingEvent.h"
#include "BActorWalkingPlayerModel.h"
#include "WalkingResultExitGame.h"

WalkingResultWin::WalkingResultWin(TSharedPtr<FJsonObject> data)
{
}

WalkingResultWin::~WalkingResultWin()
{
}

void WalkingResultWin::Execute(UBUIWalkingEvent* walking_event, ABActorWalkingPlayerModel* player_model) {
	walking_event->TextFromResult += "Zorian emerged victorious! Now, he should...\nCOMING SOON\n";
	TArray<TSharedPtr<WalkingResult>> ResultExitGame;
	ResultExitGame.Add(MakeShareable(new WalkingResultExitGame(MakeShareable(new FJsonObject()))));
	walking_event->ButtonsFromResult.Add(std::make_pair("Victory", ResultExitGame));
}