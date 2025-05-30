// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingCard.h"
#include <BActorFightingCellBase.h>

ABActorFightingUnitBase* ABActorFightingCard::SpawnUnit(ABActorFightingField* Field, ABActorFightingCellBase* Cell, TSubclassOf<ABActorFightingUnitBase> ActorToSpawnUnit)
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
    NewActor->MainMaterial = UnitMaterial;
    NewActor->SetActorRotation({ 0, 0, 0, 0 });

    NewActor->InitUnit();
    return NewActor;
}

void ABActorFightingCard::FromConfig(TSharedPtr<FightingCard> CardConfig)
{
    ManaCost = CardConfig->ManaCost;
    ID = CardConfig->ID;
    Name = CardConfig->Name;
    Cardtext = CardConfig->Cardtext;
    Type = CardConfig->Type;
    AlignmentKind = CardConfig->AlignmentKind;
    ManaGain = UStatStructs::FManaConstructor(CardConfig->AlignmentKind);
    UnitParameters = NewObject<UFightingUnitParameters>(this, UFightingUnitParameters::StaticClass());
    UnitParameters->FromFUnitParameters(CardConfig->UnitParameters);
    UnitParameters->Name = Name;
}

bool ABActorFightingCard::CanTargetCell() { return true; } // TODO IMPORTANT
bool ABActorFightingCard::CanTargetUnit() { return true; } // TODO IMPORTANT
FString ABActorFightingCard::CanBePlayed(ABActorFightingCellBase* Cell) { return ""; } // TODO IMPORTANT
