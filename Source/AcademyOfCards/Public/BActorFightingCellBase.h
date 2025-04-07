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
	static constexpr float RADIUS = 89;

	int X = 0;
	int Y = 0;
	int Z = 0;

	static int Distance(ABActorFightingCellBase* Cell1, ABActorFightingCellBase* Cell2) {
		int dx = (Cell1->X - Cell2->X), dy = (Cell1->Y - Cell2->Y), dz = (Cell1->Z - Cell2->Z);
		int adx = std::abs(dx), ady = std::abs(dy), adz = std::abs(dz);
		int dcommon = 0;
		if (dx * dy > 0 && adx <= ady) dcommon = dx;
		if (dx * dz > 0 && adx <= adz) dcommon = dx;
		if (dy * dx > 0 && ady <= adx) dcommon = dy;
		if (dy * dz > 0 && ady <= adz) dcommon = dy;
		if (dz * dx > 0 && adz <= adx) dcommon = dz;
		if (dz * dy > 0 && adz <= ady) dcommon = dz;
		return std::abs(dx - dcommon) + std::abs(dy - dcommon) + std::abs(dz - dcommon);
	}

	UFUNCTION(BlueprintCallable, Category = "Location")
	FVector GetUnitLocation();
};
