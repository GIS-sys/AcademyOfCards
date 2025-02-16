// Fill out your copyright notice in the Description page of Project Settings.


#include "BUIHUD.h"

//#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
//#include "Kismet/GameplayStatics.h"

void ABUIHUD::ShowMainMenu() {
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	MainMenu = CreateWidget<UUserWidget>(PC, MainMenuClass);

	MainMenu->AddToViewport();
}

void ABUIHUD::HideMainMenu() {
	if (MainMenu) {
		MainMenu->RemoveFromParent();
		MainMenu = nullptr;
	}
}