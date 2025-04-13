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

void ABActorFightingCard::FromConfig(TSharedPtr<FightingCard> CardConfig)
{
    ManaCost = CardConfig->ManaCost;
    ManaGain = UStatStructs::FManaConstructor(CardConfig->AlignmentKind);
    UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
    UnitParameters->CopyFrom(CardConfig->UnitParameters);
}