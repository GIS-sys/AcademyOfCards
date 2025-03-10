// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorWalkingDealer.h"
#include <BActorWalkingCard.h>
#include <BActorWalkingPlayerModel.h>
#include <WalkingCardConfig.h>
#include <WalkingDeck.h>
#include <WalkingEvent.h>
#include <Kismet/GameplayStatics.h>
#include "Serialization/JsonSerializer.h"
#include <algorithm>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <random>

// Sets default values
ABActorWalkingDealer::ABActorWalkingDealer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Deck = MakeShareable(new WalkingDeck());
	Deck->LoadConfigCards();
	Deck->LoadConfigEvents();
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

	TArray<TTuple<float, int, int>> NewDealingCardSpawnRestTime;
	bool DealtCardThisTick = false;
	for (int i = 0; i < DealingCardSpawnRestTime.Num(); ++i) {
		if (DealingCardSpawnRestTime[i].Get<2>() <= DeltaTime) {
			DealCard(DealingCardSpawnRestTime[i].Get<0>(), DealingCardSpawnRestTime[i].Get<1>());
			DealtCardThisTick = true;
			continue;
		}
		NewDealingCardSpawnRestTime.Add({ DealingCardSpawnRestTime[i].Get<0>(), DealingCardSpawnRestTime[i].Get<1>(), DealingCardSpawnRestTime[i].Get<2>() - DeltaTime });
	}
	DealingCardSpawnRestTime = NewDealingCardSpawnRestTime;

	if (DealtCardThisTick && DealingCardSpawnRestTime.IsEmpty()) {
		SetActorHiddenInGame(true);
	}
}

FVector ABActorWalkingDealer::GetCenterCellPosition(int ix, int iy)
{
	float dx = (FieldHeight - 1 - iy + FieldHeight + 1) * CardSize.Y * (1.0 + SpacingBetweenTiles);
	float dy = (ix + (FieldWidth - 1) / 2.0) * CardSize.X * (1.0 + SpacingBetweenTiles);
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ActorWhereToSpawn);
	FVector center = FoundActor->GetActorLocation();
	return center + FVector(dx, dy, 0.0) + DealingOffset;
}

void ABActorWalkingDealer::DealCard(int ix, int iy)
{
	ABActorWalkingCard* actor_wc = CardsDealt[TPair<int, int>(ix, iy)];
	actor_wc->BoardPositionX = ix;
	actor_wc->BoardPositionY = iy;
	if (TPair<int, int>(ix, iy) == StartPosition) {
		actor_wc->IsDiscovered = true;
	}
	actor_wc->SetActorHiddenInGame(false);
	FVector CellCenter = GetCenterCellPosition(ix, iy);
	actor_wc->MoveOverTimeTo(GetActorLocation(), CellCenter, TIME_CARD_DEALING_MOVEMENT);
}

void ABActorWalkingDealer::SetPlayerModel(int ix, int iy)
{
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
	ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(FoundActor);

	PlayerModel->Move(GetCenterCellPosition(ix, iy), ix, iy, this);
}

ABActorWalkingCard* ABActorWalkingDealer::CreateRandomCardFullyBlocked()
{
	AActor* actor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation());
	ABActorWalkingCard* actor_wc = dynamic_cast<ABActorWalkingCard*>(actor);

	actor_wc->DealerPtr = this;
	if (DEBUG_CARD_ID == "") {
		actor_wc->CardConfig = Deck->GetRandomCard();
	} else {
		actor_wc->CardConfig = Deck->GetCardByID(DEBUG_CARD_ID);
	}
	int MaterialIndex = 0;
	for (int i = 0; i < MaterialIDsArray.Num(); ++i) {
		if (MaterialIDsArray[i] == actor_wc->CardConfig->ID) {
			MaterialIndex = i;
			break;
		}
	}
	actor_wc->MainCardMaterial = MaterialArray[MaterialIndex];
	actor_wc->WalkingDeck = Deck;

	actor_wc->Walls.bottom = true;
	actor_wc->Walls.top = true;
	actor_wc->Walls.left = true;
	actor_wc->Walls.right = true;

	actor_wc->SetActorHiddenInGame(true);

	return actor_wc;
}

bool ABActorWalkingDealer::IsInsideFieldNoEnds(TPair<int, int> point)
{
	if (point.Get<0>() < 0 || point.Get<0>() >= FieldWidth) return false;
	if (point.Get<1>() < 0 || point.Get<1>() >= FieldHeight) return false;
	return true;
}

bool ABActorWalkingDealer::IsEnds(TPair<int, int> point)
{
	return (point == StartPosition) || (point == FinishPosition);
}

bool ABActorWalkingDealer::IsInsideFieldWithEnds(TPair<int, int> point)
{
	return IsInsideFieldNoEnds(point) || IsEnds(point);
}

template <typename T>
T shuffle(const T& array)
{
	std::vector<size_t> permutation;
	while (permutation.size() < array.size()) {
		int random_index = (rand() % (permutation.size() + 1));
		permutation.insert(permutation.begin() + random_index, permutation.size());
	}

	T new_array;
	for (int index : permutation) {
		new_array.push_back(array[index]);
	}
	return new_array;
}

TPair<int, int> operator+(const TPair<int, int>& a, const TPair<int, int>& b) {
	return TPair<int, int>(a.Get<0>() + b.Get<0>(), a.Get<1>() + b.Get<1>());
}

void ABActorWalkingDealer::CreateBoard()
{
	// create base labyrinth, fully blocked
	for (int ix = 0; ix < FieldWidth; ++ix) {
		for (int iy = 0; iy < FieldHeight; ++iy) {
			CardsDealt.Add(TPair<int, int>(ix, iy), CreateRandomCardFullyBlocked());
		}
	}
	if (!IsInsideFieldNoEnds(StartPosition)) {
		CardsDealt.Add(StartPosition, CreateRandomCardFullyBlocked());
	}
	if (!IsInsideFieldNoEnds(FinishPosition)) {
		CardsDealt.Add(FinishPosition, CreateRandomCardFullyBlocked());
	}

	// use bfs to create random pathway
	std::map<TPair<int, int>, TPair<int, int>> parents;
	std::set<TPair<int, int>> processed;
	std::deque<TPair<int, int>> bfs_queue;
	bfs_queue.push_back(StartPosition);
	parents[StartPosition] = StartPosition;
	while (!bfs_queue.empty()) {
		auto CurrentCell = bfs_queue.front();
		bfs_queue.pop_front();

		if (processed.contains(CurrentCell)) continue;
		processed.insert(CurrentCell);

		std::vector<TPair<int, int>> NextCells = {
			TPair<int, int>(CurrentCell.Get<0>() - 1, CurrentCell.Get<1>()),
			TPair<int, int>(CurrentCell.Get<0>() + 1, CurrentCell.Get<1>()),
			TPair<int, int>(CurrentCell.Get<0>(), CurrentCell.Get<1>() - 1),
			TPair<int, int>(CurrentCell.Get<0>(), CurrentCell.Get<1>() + 1),
		};
		NextCells = shuffle(NextCells);
		for (auto NextCell : NextCells) {
			if (!IsInsideFieldWithEnds(NextCell)) continue;
			if (parents.contains(NextCell)) continue;

			parents[NextCell] = CurrentCell;
			int random_index = (rand() % (bfs_queue.size() + 1));
			bfs_queue.insert(bfs_queue.begin() + random_index, NextCell);

			int dx = NextCell.Get<0>() - CurrentCell.Get<0>();
			int dy = NextCell.Get<1>() - CurrentCell.Get<1>();
			CardsDealt[CurrentCell]->Walls.Destroy(dx, dy);
			CardsDealt[NextCell]->Walls.Destroy(-dx, -dy);
		}
	}

	// delete some walls to create more pathways
	int rest_walls_amount = (FieldWidth - 1) * FieldHeight + FieldWidth * (FieldHeight - 1);
	int allowed_random_mistakes = ALLOWED_RANDOM_MISTAKES;
	for (int i = 0; i < std::min(rest_walls_amount, WALLS_TO_DELETE_AMOUNT) && allowed_random_mistakes >= 0; ++i) {
		TPair<int, int> random_cell_index { rand() % FieldWidth, rand() % FieldHeight };
		int random_direction = rand() % 2;
		TPair<int, int> random_neighbour_index = random_cell_index + TPair<int, int>(random_direction, 1 - random_direction);

		if (!IsInsideFieldNoEnds(random_neighbour_index)) {
			--allowed_random_mistakes;
			--i;
			continue;
		}

		int dx = random_neighbour_index.Get<0>() - random_cell_index.Get<0>();
		int dy = random_neighbour_index.Get<1>() - random_cell_index.Get<1>();
		if (CardsDealt[random_cell_index]->Walls.IsAllowedMovement(dx, dy)) {
			--allowed_random_mistakes;
			--i;
			continue;
		}
		CardsDealt[random_cell_index]->Walls.Destroy(dx, dy);
		CardsDealt[random_neighbour_index]->Walls.Destroy(-dx, -dy);
	}
}

bool ABActorWalkingDealer::CheckAbleToGo(int CurrentBoardPositionX, int CurrentBoardPositionY, int BoardPositionX, int BoardPositionY)
{
	// walk only 1 step
	int dx = BoardPositionX - CurrentBoardPositionX;
	int dy = BoardPositionY - CurrentBoardPositionY;
	if (dx * dx + dy * dy > 1) {
		return false;
	}
	// don't go out of bounds
	if (!IsInsideFieldWithEnds({ BoardPositionX, BoardPositionY })) return false;
	if (!IsInsideFieldWithEnds({ CurrentBoardPositionX, CurrentBoardPositionY })) return false;
	// don't walk through walls
	auto CurrentCellWalls = CardsDealt[TPair<int, int>(CurrentBoardPositionX, CurrentBoardPositionY)]->Walls;
	if (!CurrentCellWalls.IsAllowedMovement(dx, dy)) {
		return false;
	}
	// by default return true
	return true;
}

void ABActorWalkingDealer::SetTimersForCardDeal()
{
	float TimeBetweenFactor = 1100.0 / 16.0; // Idk why but I need this factor to make TIME_BETWEEN_CARD_DEAL be in seconds
	for (int ix = 0; ix < FieldWidth; ++ix) {
		for (int iy = 0; iy < FieldHeight; ++iy) {
			DealingCardSpawnRestTime.Add({ ix, iy, (ix + (FieldHeight - 1 - iy)) * TimeBetweenFactor * TIME_BETWEEN_CARD_DEAL });
		}
	}
	if (!IsInsideFieldNoEnds(StartPosition)) {
		int ix = StartPosition.Get<0>();
		int iy = StartPosition.Get<1>();
		DealingCardSpawnRestTime.Add({ ix, iy, (ix + (FieldHeight - 1 - iy)) * TimeBetweenFactor * TIME_BETWEEN_CARD_DEAL });
	}
	if (!IsInsideFieldNoEnds(FinishPosition)) {
		int ix = FinishPosition.Get<0>();
		int iy = FinishPosition.Get<1>();
		DealingCardSpawnRestTime.Add({ ix, iy, (ix + (FieldHeight - 1 - iy)) * TimeBetweenFactor * TIME_BETWEEN_CARD_DEAL });
	}
}

void ABActorWalkingDealer::DealCards()
{
	SetPlayerModel(StartPosition.Get<0>(), StartPosition.Get<1>());

	CreateBoard();

	SetTimersForCardDeal();
}