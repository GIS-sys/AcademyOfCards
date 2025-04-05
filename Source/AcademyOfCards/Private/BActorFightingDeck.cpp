// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingDeck.h"

void ABActorFightingDeck::DealCards()
{
    for (int i = 0; i < STARTING_HAND; ++i) {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this, i]() { DrawCard(i); },
            DRAWING_TIME * i + 0.001,
            false
        );
    }
}

void ABActorFightingDeck::DrawCard(int index)
{
    UE_LOG(LogTemp, Error, TEXT("DrawCard: %d"), index);
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
        ActorToSpawnCard,
        FVector(0, 0, 0),
        GetActorRotation(),
        SpawnParams
    );
    ABActorFightingCard* NewActor = dynamic_cast<ABActorFightingCard*>(NewActorRaw);
    NewActor->SetActorLocation(GetActorLocation());
    CardActors.Add(NewActor);

    for (int i = 0; i < CardActors.Num(); ++i) {
        const auto& card = CardActors[i];
        card->MoveOverTimeTo(
            card->GetActorLocation(),
            GetActorLocation() + WHERE_IS_HAND + (i - CardActors.Num() / 2.0) * SPACE_BETWEEN_CARDS,
            DRAWING_TIME
        );
    }
}