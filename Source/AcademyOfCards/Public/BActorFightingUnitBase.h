// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include <BActorFightingCellBase.h>
#include "BActorFightingUnitBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingUnitBase : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	ABActorFightingCellBase* CurrentCell;

	float MOVING_TIME = 1.0;

	void Move(ABActorFightingCellBase* Cell);
};
