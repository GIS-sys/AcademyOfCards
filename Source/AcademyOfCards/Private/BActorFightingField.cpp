// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingField.h"

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

void ABActorFightingField::MoveUnit(ABActorFightingUnitBase* Unit)
{
    // TODO
}

void ABActorFightingField::InitPlayers()
{
    IsPlayerTurn = false;

    PlayerMana.GeneralMax = 0;
    OpponentMana.GeneralMax = 0;

    // TODO use who we are fighting
    OpponentStats.Health = 10;

    PassTurn();
}

void ABActorFightingField::InitUnits()
{

}

void ABActorFightingField::Init()
{
    InitPlayers();
    InitCells();
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

    if (!IsPlayerTurn) {
        PassTurn(); //AIMove(); // TODO
    }
    return "";
}
