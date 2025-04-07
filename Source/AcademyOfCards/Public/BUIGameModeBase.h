// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BActorEnhanced.h"
#include "BUIGameModeBase.generated.h"
class WalkingResultFight;
class ABActorFightingField;

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABUIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	const float MOVE_OUT_OF_FRAME_SECONDS = 2.0;

public:
	void SwitchToFight(WalkingResultFight* FightResult);
	UFUNCTION(BlueprintCallable, Category = "Transition")
	void SwitchToWalking(ABActorFightingField* FightField);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	TSubclassOf<ABUIGameModeBase> PlayerGameModeWalkingStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	TSubclassOf<ABUIGameModeBase> PlayerGameModeFightStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transition")
	TArray<TSubclassOf<ABActorEnhanced>> MoveWhileStageTransitioningStaticClass;
};
