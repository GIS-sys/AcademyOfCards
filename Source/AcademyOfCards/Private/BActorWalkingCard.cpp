// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingCard.h"
#include <WalkingEvent.h>
#include <BActorWalkingPlayerModel.h>
#include <Kismet/GameplayStatics.h>
#include "WalkingCardConfig.h"
#include "UMyGameInstance.h"

// Sets default values
ABActorWalkingCard::ABActorWalkingCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABActorWalkingCard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABActorWalkingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(LocationOriginal + LocationDelta);
	SetActorRelativeScale3D(ScaleRelative);
}

int ABActorWalkingCard::MoveTo() {
	TSharedPtr<WalkingConfigs> Configs = Cast<UUMyGameInstance>(GetGameInstance())->LoadedWalkingConfigs;

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);
	if (PlayerModel->Move(GetActorLocation(), BoardPositionX, BoardPositionY, DealerPtr)) {
		if (!IsDiscovered) {
			IsDiscovered = true;
			IsOpenlyVisible = true;
			DealerPtr->UpdateOpenlyVisible(this);
			Configs->GetEventByID(CardConfig->GetEventFired(PlayerModel->PlayerStats))->Fire(DealerPtr, this);
			return 1;
		} else {
			return 2;
		}
	}
	return 0;
}

bool ABActorWalkingCard::IsCollectible() {
	char last_id_letter = CardConfig->ID[CardConfig->ID.Len() - 1];
	return last_id_letter == 'c';
}

LevelSaveInstance ABActorWalkingCard::Save() {
	LevelSaveInstance SaveInstance;
	SaveInstance.SetCopy("Walls", Walls);
	SaveInstance.SetCopy("IsCloseUpLook", IsCloseUpLook);
	SaveInstance.SetCopy("IsDiscovered", IsDiscovered);
	SaveInstance.SetCopy("IsOpenlyVisible", IsOpenlyVisible);
	SaveInstance.SetCopy("ID", CardConfig->ID);
	return SaveInstance;
};

void ABActorWalkingCard::Load(LevelSaveInstance* SaveInstance) {
	Walls = SaveInstance->GetAsCopy<WallsStruct>("Walls");
	IsCloseUpLook = SaveInstance->GetAsCopy<bool>("IsCloseUpLook");
	IsDiscovered = SaveInstance->GetAsCopy<bool>("IsDiscovered");
	IsOpenlyVisible = SaveInstance->GetAsCopy<bool>("IsOpenlyVisible");

	FString ID = SaveInstance->GetAsCopy<FString>("ID");
	TSharedPtr<WalkingConfigs> Configs = Cast<UUMyGameInstance>(GetGameInstance())->LoadedWalkingConfigs;
	CardConfig = Configs->GetCardByID(ID);
	MainCardMaterial = DealerPtr->GetMaterialByID(ID);
};