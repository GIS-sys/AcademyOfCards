// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingCard.h"
#include <BActorFightingCellBase.h>

ABActorFightingUnitBase* ABActorFightingCard::SpawnUnit(ABActorFightingCellBase* Cell, TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
        ActorToSpawnUnit,
        FVector(0, 0, 0),
        GetActorRotation(),
        SpawnParams
    );
    ABActorFightingUnitBase* NewActor = dynamic_cast<ABActorFightingUnitBase*>(NewActorRaw);
    NewActor->LocationOriginal = Cell->GetUnitLocation();
    NewActor->CurrentCell = Cell;
    NewActor->UnitParameters = UnitParameters;
    NewActor->IsControlledByPlayer = IsControlledByPlayer;
    NewActor->OnSpawn();
    return NewActor;
}

void ABActorFightingCard::FromConfig(TSharedPtr<FightingCard> CardConfig)  // TODOIMPORTANT
{
    ManaCost = FMana(rand() % 6, rand() % 2, rand() % 2, rand() % 2, rand() % 2); // TODO proper card parameters ^
    ManaGain = FMana(0, rand() % 2, rand() % 2, rand() % 2, rand() % 2);
    UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
    UnitParameters->Health = rand() % 10 + 1;
    UnitParameters->Power = rand() % 10 + 1;
    UnitParameters->Movement = rand() % 2 + 1;
    UnitParameters->Attacks = rand() % 2 + 1;
    UnitParameters->Range = rand() % 2 + 1;
    UnitParameters->Abilities = {};
}