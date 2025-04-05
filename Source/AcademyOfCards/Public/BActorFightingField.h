// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <BActorEnhanced.h>
#include <BActorFightingUnitBase.h>
#include "BActorFightingField.generated.h"

UCLASS()
class ACADEMYOFCARDS_API ABActorFightingField : public ABActorEnhanced
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TArray<TSubclassOf<ABActorEnhanced>> CardsSubclasses;

	TArray<ABActorEnhanced*> Cards;

	TArray<ABActorFightingUnitBase*> Units;
};
