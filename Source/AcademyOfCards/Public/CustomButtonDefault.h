// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomButtonDefault.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API UCustomButtonDefault : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	FString InnerText;
};
