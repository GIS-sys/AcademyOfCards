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
#include "BActorWalkingDealer.h"
#include "LevelSaveInstance.h"


void ABUIGameModeBase::SwitchToFight(WalkingResultFight* FightResult) {
	AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);
	AActor* DealerRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingDealer::StaticClass());
	ABActorWalkingDealer* Dealer = Cast<ABActorWalkingDealer>(DealerRaw);

	// Change data in persistent stage
	UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(GetGameInstance());
	MyGameInstance->PersistentStage = EnumStage::FIGHTING;

	MyGameInstance->WalkingSave.Saves.Add(UUMyGameInstance::SAVE_WALKING_PLAYER_STATS, UStatStructs::SavePlayerStats(PlayerModel->PlayerStats));
	MyGameInstance->WalkingSave.Saves.Add(UUMyGameInstance::SAVE_WALKING_FIGHT_RESULT, FightResult->Save());
	MyGameInstance->WalkingSave.Saves.Add(UUMyGameInstance::SAVE_WALKING_DEALER, Dealer->Save());
	MyGameInstance->WalkingSave.Saves.Add(UUMyGameInstance::SAVE_WALKING_PLAYER_MODEL, PlayerModel->Save());

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

	MyGameInstance->FightingSave.Saves.Add(UUMyGameInstance::SAVE_FIGHTING_PLAYER_STATS, UStatStructs::SavePlayerStats(PlayerModel->PlayerStats));
	MyGameInstance->FightingSave.Saves.Add(UUMyGameInstance::SAVE_FIGHTING_FIGHT_OUTCOME, LevelSaveInstance(FightField->IsPlayerWinner));

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