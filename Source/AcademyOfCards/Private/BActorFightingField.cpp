// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingField.h"
#include <UMyGameInstance.h>
#include <WalkingResultFight.h>
#include <Kismet/GameplayStatics.h>
#include "BActorWalkingPlayerModel.h"

FPlayerStats* ABActorFightingField::GetPlayerStats(bool IsPlayerMe) {
    if (!IsPlayerMe) return &OpponentStats;

    AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
    ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);
    return &PlayerModel->PlayerStats;
}

void ABActorFightingField::InitCells()
{
    for (int i = 0; i < RADIUS; ++i) {
        ArrayCells.Add(TArray<TArray<ABActorFightingCellBase*>>());
        for (int j = 0; j < RADIUS; ++j) {
            ArrayCells[i].Add(TArray<ABActorFightingCellBase*>());
            for (int k = 0; k < RADIUS; ++k) {
                ArrayCells[i][j].Add(nullptr);
            }
        }
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    FVector vi(-1.732 / 2, 0.5, 0);
    FVector vj(1.732 / 2, 0.5, 0);
    FVector vk(0, -1, 0);
    for (int i = 0; i < RADIUS; ++i) {
        for (int j = 0; j < RADIUS; ++j) {
            for (int k = 0; k < RADIUS; ++k) {
                if (i != 0 && j != 0 && k != 0) continue;
                AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
                    ActorToSpawnCells,
                    FVector(0, 0, 0),
                    GetActorRotation(),
                    SpawnParams
                );
                ABActorFightingCellBase* NewActor = dynamic_cast<ABActorFightingCellBase*>(NewActorRaw);
                NewActor->AttachToComponent(
                    SceneComponentCells,
                    FAttachmentTransformRules::SnapToTargetNotIncludingScale
                );
                NewActor->LocationOriginal = (
                    SceneComponentCells->GetComponentLocation() +
                    (i * vi + j * vj + k * vk) * ABActorFightingCellBase::RADIUS +
                    FVector(-10, 0, -COLUMN_SHOW_HEIGHT)
                    //+ FVector(0, 0, -20 * rand() * 1.0 / RAND_MAX)
                );
                ArrayCells[i][j][k] = NewActor;
                NewActor->X = i;
                NewActor->Y = j;
                NewActor->Z = k;
            }
        }
    }
}

void ABActorFightingField::InitDecks()
{
    DeckMy->BActorFightingField = this;
    DeckOpponent->BActorFightingField = this;
    DeckMy->Init();
    DeckOpponent->Init();
    DeckMy->DealCards();
    DeckOpponent->DealCards();
}

bool ABActorFightingField::MoveUnit(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell)
{
    if (IsOccupied(Cell)) return false;
    return Unit->Move(Cell);
}

bool ABActorFightingField::AttackUnit(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim)
{
    if (Attacker == Victim) return false;
    if (Attacker->UnitParameters->CurrentAttacks <= 0) return false;
    int Distance = ABActorFightingCellBase::Distance(Attacker->CurrentCell, Victim->CurrentCell);
    if (Distance > Attacker->UnitParameters->Range) return false;

    Victim->UnitParameters->CurrentHealth -= Attacker->UnitParameters->CurrentPower;
    Attacker->UnitParameters->CurrentAttacks -= 1;
    if (Victim->IsPlayer) {
        Victim->UnitParameters->Health = Victim->UnitParameters->CurrentHealth;
        GetPlayerStats(Victim->IsControlledByPlayer)->Health = Victim->UnitParameters->Health;
    }

    if (Victim->UnitParameters->CurrentHealth <= 0) {
        for (int i = 0; i < ArrayUnits.Num(); ++i) {
            if (ArrayUnits[i] == Victim) ArrayUnits.RemoveAt(i);
        }
        if (!Victim->IsPlayer) {
            Victim->Destroy();
        } else {
            IsFinished = true;
            IsPlayerWinner = (Victim != PlayerUnitMy);
        }
    }
    return true;
}

bool ABActorFightingField::PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    if (ABActorFightingCellBase::Distance(GetCurrentPlayerUnit()->CurrentCell, Cell) > 1) return false;
    if (IsOccupied(Cell)) return false;
    FMana ManaRest = PlayerMana - Card->ManaCost;
    if (!ManaRest) return false;
    ABActorFightingUnitBase* NewUnit = DeckMy->PlayCard(Card, Cell);
    if (!NewUnit) return false;
    ArrayUnits.Add(NewUnit);
    PlayerMana -= Card->ManaCost;
    PlayerMana += Card->ManaGain;
    return true;
}

void ABActorFightingField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    for (ABActorFightingUnitBase* Unit : ArrayUnits) {
        if (Unit && !Unit->IsMovingOverTime()) Unit->MoveOverTimeTo(Unit->LocationOriginal, Unit->CurrentCell->GetUnitLocation(), 0.1);
    }
}

void ABActorFightingField::InitPlayers()
{
    IsPlayerTurn = false;

    PlayerMana.GeneralMax = 0;
    OpponentMana.GeneralMax = 0;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    {
        AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
            ActorToSpawnUnit,
            FVector(0, 0, 0),
            GetActorRotation(),
            SpawnParams
        );
        PlayerUnitMy = dynamic_cast<ABActorFightingUnitBase*>(NewActorRaw);
        ABActorFightingCellBase* StartingCell = ArrayCells[PLAYER_START_MY_X][PLAYER_START_MY_Y][PLAYER_START_MY_Z];
        PlayerUnitMy->InitPlayerMy(StartingCell, GetPlayerStats(true));
        ArrayUnits.Add(PlayerUnitMy);
    }
    {
        AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
            ActorToSpawnUnit,
            FVector(0, 0, 0),
            GetActorRotation(),
            SpawnParams
        );
        PlayerUnitOpponent = dynamic_cast<ABActorFightingUnitBase*>(NewActorRaw);
        ABActorFightingCellBase* StartingCell = ArrayCells[PLAYER_START_OPPONENT_X][PLAYER_START_OPPONENT_Y][PLAYER_START_OPPONENT_Z];
        PlayerUnitOpponent->InitPlayerOpponent(OpponentName, StartingCell, GetPlayerStats(false));
        ArrayUnits.Add(PlayerUnitOpponent);
    }

    PassTurn();
}

void ABActorFightingField::InitUnits()
{

}

void ABActorFightingField::InitLoadFromWalking()
{
    // Load
    UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(GetGameInstance());
    FPlayerStats WalkingPlayerStats = UStatStructs::LoadPlayerStats(MyGameInstance->WalkingSave.Saves.Find(UUMyGameInstance::SAVE_WALKING_PLAYER_STATS));
    WalkingResultFight* WalkingFight = WalkingResultFight::Load(MyGameInstance->WalkingSave.Saves.Find(UUMyGameInstance::SAVE_WALKING_FIGHT_RESULT));
    
    // Set back
    OpponentName = WalkingFight->GetOpponent();

    *GetPlayerStats(true) = WalkingPlayerStats;
}

void ABActorFightingField::Init()
{
    InitLoadFromWalking();

    InitCells();
    InitPlayers();
    InitDecks();
    InitUnits();
}



FString ABActorFightingField::AbilityDrawCard()
{
    if (PlayerMana.General >= 4) {
        PlayerMana.General -= 4;
        DeckMy->DrawCard(0);
        return "";
    }
    return "Not enough mana";
}

FString ABActorFightingField::AbilityGetMana(int& Mana)
{
    if (PlayerMana.General >= 2) {
        PlayerMana.General -= 2;
        Mana += 1;
        return "";
    }
    return "Not enough mana";
}

FString ABActorFightingField::AbilityGetManaLight()
{
    return AbilityGetMana(PlayerMana.Light);
}

FString ABActorFightingField::AbilityGetManaDark()
{
    return AbilityGetMana(PlayerMana.Dark);
}

FString ABActorFightingField::AbilityGetManaFire()
{
    return AbilityGetMana(PlayerMana.Fire);
}

FString ABActorFightingField::AbilityGetManaIce()
{
    return AbilityGetMana(PlayerMana.Ice);
}

FString ABActorFightingField::PassTurn()
{
    IsPlayerTurn = !IsPlayerTurn;

    FPlayerMana* CurrentPlayerMana;
    if (IsPlayerTurn) {
        CurrentPlayerMana = &PlayerMana;
    } else {
        CurrentPlayerMana = &OpponentMana;
    }
    
    CurrentPlayerMana->GeneralMax += 2;
    CurrentPlayerMana->General += CurrentPlayerMana->GeneralMax;
    if (CurrentPlayerMana->General > CurrentPlayerMana->GeneralMax) CurrentPlayerMana->General = CurrentPlayerMana->GeneralMax;

    for (ABActorFightingUnitBase* Unit : ArrayUnits) {
        Unit->OnTurnEnd(IsPlayerTurn != Unit->IsControlledByPlayer);
    }

    if (!IsPlayerTurn) {
        AIOpponent.YourTurn(this);
    }
    return "";
}








void AI::StartThinkingLoop(ABActorFightingField* FightingField) {
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, FightingField]() {
        this->Think(FightingField);

        AsyncTask(ENamedThreads::GameThread, [this, FightingField]() {
            this->Act(FightingField);
            if (this->HasFinishedTurn(FightingField)) {
                FightingField->PassTurn();
            }
            else {
                this->StartThinkingLoop(FightingField);
            }
            });
        });
}

void AI::YourTurn(ABActorFightingField* FightingField) {
    InitNextTurn(FightingField);
    StartThinkingLoop(FightingField);
}

// TODO implement AI below
void AI::InitNextTurn(ABActorFightingField* FightingField)
{
    MovingPlayerCoordinates = { 0, 0, 0 };
}

void AI::Think(ABActorFightingField* FightingField)
{
    FPlatformProcess::Sleep(2.0f);
    
    TTuple<int, int, int> CurrentCoordinates = FightingField->PlayerUnitOpponent->CurrentCell->GetCoordinates();
    TArray<TTuple<int, int, int>> NeighboursCoordinates = FightingField->PlayerUnitOpponent->CurrentCell->GetNeighboursCoordinates(FightingField->RADIUS);
    for (auto t : NeighboursCoordinates) {
        UE_LOG(LogTemp, Error, TEXT("AI possible neighbour: %d %d %d"), t.Get<0>(), t.Get<1>(), t.Get<2>());
    }
    MovingPlayerCoordinates = NeighboursCoordinates[1];
}

void AI::Act(ABActorFightingField* FightingField)
{
    auto [x, y, z ] = MovingPlayerCoordinates;
    UE_LOG(LogTemp, Error, TEXT("AI acting by walking to %d %d %d"), x, y, z);
    FightingField->MoveUnit(FightingField->PlayerUnitOpponent, FightingField->ArrayCells[x][y][z]);
}

bool AI::HasFinishedTurn(ABActorFightingField* FightingField)
{
    return FightingField->PlayerUnitOpponent->UnitParameters->CurrentMovement == 0;
}