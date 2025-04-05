// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BActorEnhanced.h>
#include <BActorFightingUnitBase.h>
#include <BActorFightingCellBase.h>
#include "BActorFightingField.generated.h"

UCLASS()
class ACADEMYOFCARDS_API ABActorFightingField : public ABActorEnhanced
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<ABActorEnhanced>> CardsSubclasses;

	TArray<ABActorEnhanced*> ArrayCards;

	TArray<ABActorFightingUnitBase*> ArrayUnits;

	TArray<TArray<TArray<ABActorFightingCellBase*>>> ArrayCells;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ABActorFightingCellBase> ActorToSpawnCells;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* SceneComponentCells;

	UFUNCTION(BlueprintCallable, Category = "Base")
	void Init();

	const int RADIUS = 5;
};
