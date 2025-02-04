// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BUIHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABUIHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void HideMainMenu();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> MainMenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainMenu;
};
