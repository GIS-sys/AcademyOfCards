// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BUIGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABUIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	const float MOVE_OUT_OF_FRAME_SECONDS = 1.0;

public:
	void SwitchToFight();
};
