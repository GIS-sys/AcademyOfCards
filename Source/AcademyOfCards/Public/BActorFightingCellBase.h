// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include "BActorFightingCellBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingCellBase : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	static constexpr float RADIUS = 80;

	UFUNCTION(BlueprintCallable, Category = "Location")
	FVector GetUnitLocation();
};
