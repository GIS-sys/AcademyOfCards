// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIGameModeBase.h"
#include <BPlayerControllerBase.h>
#include "BUIHUD.h"
#include <BPlayerPawnBase.h>
#include <UMyGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include "BActorWalkingPlayerModel.h"
#include "WalkingResultFight.h"
#include "BActorFightingField.h"
#include "BActorEnhanced.h"


void ABUIGameModeBase::SwitchToFight(WalkingResultFight* FightResult) {
	AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);

	// Change data in persistent stage
	UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(GetGameInstance());
	MyGameInstance->PersistentStage = EnumStage::FIGHTING;

	MyGameInstance->WalkingSave.Data.Add("PlayerStats", new TSharedPtr<FPlayerStats>(new FPlayerStats(PlayerModel->PlayerStats)));
	MyGameInstance->WalkingSave.Data.Add("FightResult", new TSharedPtr<WalkingResultFight>(new WalkingResultFight(*FightResult)));

	UE_LOG(LogTemp, Error, TEXT("Switch To Fight"));

	// Move objects out of frame
	for (const auto& ClassToMove : MoveWhileStageTransitioningStaticClass) {
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToMove, FoundActors);
		for (AActor* Actor : FoundActors) {
			ABActorEnhanced* ActorToMove = Cast<ABActorEnhanced>(Actor);
			ActorToMove->MoveOutOfFrame(MOVE_OUT_OF_FRAME_SECONDS);
		}
	}
	ABPlayerControllerBase* CurrentPlayerController = Cast<ABPlayerControllerBase>(GetWorld()->GetFirstPlayerController());
	CurrentPlayerController->Disappear(MOVE_OUT_OF_FRAME_SECONDS);
	ABUIHUD* CurrentHud = Cast<ABUIHUD>(CurrentPlayerController->GetHUD());
	CurrentHud->Disappear(MOVE_OUT_OF_FRAME_SECONDS);
	ABPlayerPawnBase* CurrentPawn = Cast<ABPlayerPawnBase>(CurrentPlayerController->GetPawn());
	CurrentPawn->SwitchToFight(MOVE_OUT_OF_FRAME_SECONDS);

	// Switch level
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[&]() {
			// Save current level
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABActorEnhanced::StaticClass(), FoundActors);

			// Transition to another level
			FString LevelName = "WalkingStage";
			TSubclassOf<AGameModeBase> GameModeToUse = PlayerGameModeFightStage;
			FString LevelURL = FString::Printf(TEXT("%s?GameMode=%s"), *LevelName, *GameModeToUse->GetPathName());
			UGameplayStatics::OpenLevel(this, *LevelName, true, "game=" + GameModeToUse->GetPathName());
		},
		MOVE_OUT_OF_FRAME_SECONDS,
		false
	);
}

void ABUIGameModeBase::SwitchToWalking(ABActorFightingField* FightField)
{
	AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);

	// Change data in persistent stage
	UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(GetGameInstance());
	MyGameInstance->PersistentStage = EnumStage::WALKING;
	MyGameInstance->NotLoadedWalkingBefore = false;

	MyGameInstance->WalkingSave.Data.Add("PlayerStats", new TSharedPtr<FPlayerStats>(new FPlayerStats(PlayerModel->PlayerStats)));
	MyGameInstance->WalkingSave.Data.Add("FightOutcome", new TSharedPtr<bool>(new bool(FightField->IsPlayerWinner))); // TODO use that

	UE_LOG(LogTemp, Error, TEXT("Switch To Walking"));

	// Switch level
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[&]() {
			// Save current level
			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABActorEnhanced::StaticClass(), FoundActors);

			// Transition to another level
			FString LevelName = "WalkingStage";
			TSubclassOf<AGameModeBase> GameModeToUse = PlayerGameModeWalkingStage;
			FString LevelURL = FString::Printf(TEXT("%s?GameMode=%s"), *LevelName, *GameModeToUse->GetPathName());
			UGameplayStatics::OpenLevel(this, *LevelName, true, "game=" + GameModeToUse->GetPathName());
		},
		MOVE_OUT_OF_FRAME_SECONDS,
		false
	);
}