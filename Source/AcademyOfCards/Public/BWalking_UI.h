// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BUIWalkingEvent.h"
#include "BWalking_UI.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API UBWalking_UI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	UBUIWalkingEvent* BUIWalkingEvent;
};
