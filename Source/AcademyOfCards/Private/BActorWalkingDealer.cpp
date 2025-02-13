// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingDealer.h"
#include <BActorWalkingCard.h>
#include <WalkingEvent.h>
#include "Serialization/JsonSerializer.h"

// Sets default values
ABActorWalkingDealer::ABActorWalkingDealer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LoadConfigEvents();
}

void ABActorWalkingDealer::LoadConfigEvents() {
	FString FilePath = FPaths::ProjectContentDir() / TEXT("WalkingStage/Configs/config_walking_events.json");
	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			for (const auto& [x, y] : JsonObject->Values) {
				Events.Add(MakeShareable(new WalkingEvent(x, y->AsObject())));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from path: %s"), *FilePath);
	}
}

// Called when the game starts or when spawned
void ABActorWalkingDealer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABActorWalkingDealer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<TTuple<float, int, int>> NewDealingCardSpawinRestTime;
	for (int i = 0; i < DealingCardSpawinRestTime.Num(); ++i) {
		if (DealingCardSpawinRestTime[i].Get<2>() <= DeltaTime) {
			DealCard(DealingCardSpawinRestTime[i].Get<0>(), DealingCardSpawinRestTime[i].Get<1>());
			continue;
		}
		NewDealingCardSpawinRestTime.Add({ DealingCardSpawinRestTime[i].Get<0>(), DealingCardSpawinRestTime[i].Get<1>(), DealingCardSpawinRestTime[i].Get<2>() - DeltaTime });
	}
	DealingCardSpawinRestTime = NewDealingCardSpawinRestTime;
}

void ABActorWalkingDealer::DealCard(int ix, int iy)
{
	AActor* actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation());
	ABActorWalkingCard* actor_wc = dynamic_cast<ABActorWalkingCard*>(actor);
	FVector size = actor->GetComponentsBoundingBox(false, true).GetSize();
	float dx = ix * size.X * (1.0 + SpacingBetweenTiles);
	float dy = iy * size.Y * (1.0 + SpacingBetweenTiles);

	actor_wc->MainCardMaterial = MaterialArray[FMath::Rand() % MaterialArray.Num()];
	actor_wc->Event = Events[FMath::Rand() % Events.Num()];
	//actor_wc->LocationOriginal = GetActorLocation() + FVector(dx, dy, 0.0) + DealingOffset;
	//actor->SetActorLocation(actor_wc->LocationOriginal);
	actor_wc->MoveOverTimeTo(GetActorLocation(), GetActorLocation() + FVector(dx, dy, 0.0) + DealingOffset, 1.0);
}

void ABActorWalkingDealer::DealCards()
{
	for (int ix = 0; ix < 4; ++ix) {
		for (int iy = 0; iy < 6; ++iy) {
			DealingCardSpawinRestTime.Add({ ix, iy, (ix + (5 - iy)) * 10 });
		}
	}
	DealingCardSpawinRestTime.Add({ -1, 3, (4 + 6 + 5) * 10 });
	DealingCardSpawinRestTime.Add({ 4, 2, (4 + 6 + 5) * 10 });
}