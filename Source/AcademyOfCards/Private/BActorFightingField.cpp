// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingField.h"
#include <UMyGameInstance.h>
#include <WalkingResultFight.h>
#include <Kismet/GameplayStatics.h>
#include "BActorWalkingPlayerModel.h"

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
    for (ABActorFightingUnitBase* AnotherUnit : ArrayUnits) {
        if (AnotherUnit->CurrentCell == Cell) return false;
    }
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
    return true;
}

void ABActorFightingField::PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    FMana ManaRest = PlayerMana - Card->ManaCost;
    if (ManaRest) {
        ABActorFightingUnitBase* NewUnit = DeckMy->PlayCard(Card, Cell);
        ArrayUnits.Add(NewUnit);
        PlayerMana -= Card->ManaCost;
        PlayerMana += Card->ManaGain;
    }
}

void ABActorFightingField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    for (ABActorFightingUnitBase* Unit : ArrayUnits) {
        if (Unit && !Unit->IsMovingOverTime()) Unit->MoveOverTimeTo(Unit->LocationOriginal, Unit->CurrentCell->GetUnitLocation(), 0.1);
    }
    for (ABActorFightingUnitBase* Unit : {PlayerUnitMy, PlayerUnitOpponent}) {
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
        AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
        ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);

        AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
            ActorToSpawnUnit,
            FVector(0, 0, 0),
            GetActorRotation(),
            SpawnParams
        );
        PlayerUnitMy = dynamic_cast<ABActorFightingUnitBase*>(NewActorRaw);
        ABActorFightingCellBase* StartingCell = ArrayCells[PLAYER_START_MY_X][PLAYER_START_MY_Y][PLAYER_START_MY_Z];
        PlayerUnitMy->InitPlayerMy(StartingCell, &PlayerModel->PlayerStats);
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
        PlayerUnitOpponent->InitPlayerOpponent(OpponentName, StartingCell, &OpponentStats);
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
    TSharedPtr<FPlayerStats>* WalkingPlayerStats = (TSharedPtr<FPlayerStats>*)(MyGameInstance->WalkingSave.Data["PlayerStats"]);
    TSharedPtr<WalkingResultFight>* WalkingFight = (TSharedPtr<WalkingResultFight>*)(MyGameInstance->WalkingSave.Data["FightResult"]);
    
    // Set back
    OpponentName = (*WalkingFight)->GetOpponent();

    AActor* PlayerModelRaw = UGameplayStatics::GetActorOfClass(GetWorld(), ABActorWalkingPlayerModel::StaticClass());
    ABActorWalkingPlayerModel* PlayerModel = Cast<ABActorWalkingPlayerModel>(PlayerModelRaw);
    PlayerModel->PlayerStats = **WalkingPlayerStats;
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
        PassTurn(); //AIMove(); // TODO
    }
    return "";
}
