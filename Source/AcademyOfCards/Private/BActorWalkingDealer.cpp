// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingDealer.h"
#include <BActorWalkingCard.h>
#include <BActorWalkingPlayerModel.h>
#include <WalkingEvent.h>
#include <Kismet/GameplayStatics.h>
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

FVector ABActorWalkingDealer::GetCenterCellPosition(int ix, int iy)
{
	float dx = ix * CardSize.X * (1.0 + SpacingBetweenTiles);
	float dy = iy * CardSize.Y * (1.0 + SpacingBetweenTiles);
	return GetActorLocation() + FVector(dx, dy, 0.0) + DealingOffset;
}

void ABActorWalkingDealer::DealCard(int ix, int iy)
{
	AActor* actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation());
	ABActorWalkingCard* actor_wc = dynamic_cast<ABActorWalkingCard*>(actor);

	actor_wc->BoardPositionX = ix;
	actor_wc->BoardPositionY = iy;
	actor_wc->DealerPtr = this;
	actor_wc->MainCardMaterial = MaterialArray[FMath::Rand() % MaterialArray.Num()];
	actor_wc->Event = Events[FMath::Rand() % Events.Num()];
	//actor_wc->LocationOriginal = GetActorLocation() + FVector(dx, dy, 0.0) + DealingOffset;
	//actor->SetActorLocation(actor_wc->LocationOriginal);

	actor_wc->MoveOverTimeTo(GetActorLocation(), GetCenterCellPosition(ix, iy), 1.0);

	actor_wc->Walls.bottom = (bool)(std::rand() % 2); // TODO
	actor_wc->Walls.top = (bool)(std::rand() % 2); // TODO
	actor_wc->Walls.left = (bool)(std::rand() % 2); // TODO
	actor_wc->Walls.right = (bool)(std::rand() % 2); // TODO

	CardsDealt.Add(TPair<int, int>(ix, iy), actor_wc);
}

void ABActorWalkingDealer::SetPlayerModel(int ix, int iy)
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);

	PlayerModel->Move(GetCenterCellPosition(ix, iy), ix, iy, this);
}

void ABActorWalkingDealer::CreateBoard()
{
	// TODO
}

bool ABActorWalkingDealer::CheckAbleToGo(int CurrentBoardPositionX, int CurrentBoardPositionY, int BoardPositionX, int BoardPositionY)
{
	int dx = CurrentBoardPositionX - BoardPositionX;
	int dy = CurrentBoardPositionY - BoardPositionY;
	if (dx * dx + dy * dy > 1) {
		return false;
	}
	// TODO
	return true;
}

void ABActorWalkingDealer::DealCards()
{
	CreateBoard();

	// TODO
	SetPlayerModel(StartPosition.Get<1>(), StartPosition.Get<0>());
	for (int ix = 0; ix < FieldHeight; ++ix) {
		for (int iy = 0; iy < FieldWidth; ++iy) {
			DealingCardSpawinRestTime.Add({ ix, iy, (ix + (FieldHeight - 1 - iy)) * 10 });
		}
	}
	if (!(0 <= StartPosition.Get<1>() && StartPosition.Get<1>() < FieldHeight) || !(0 <= StartPosition.Get<0>() && StartPosition.Get<0>() < FieldWidth)) {
		DealingCardSpawinRestTime.Add({ StartPosition.Get<1>(), StartPosition.Get<0>(), (FieldHeight + FieldWidth) * 10 });
	}
	if (!(0 <= FinishPosition.Get<1>() && FinishPosition.Get<1>() < FieldHeight) || !(0 <= FinishPosition.Get<0>() && FinishPosition.Get<0>() < FieldWidth)) {
		DealingCardSpawinRestTime.Add({ FinishPosition.Get<1>(), FinishPosition.Get<0>(), (FieldHeight + FieldWidth) * 10 });
	}
}