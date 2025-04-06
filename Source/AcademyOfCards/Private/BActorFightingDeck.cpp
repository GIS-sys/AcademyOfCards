// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingDeck.h"
#include <BActorFightingField.h>

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
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    AActor* NewActorRaw = GetWorld()->SpawnActor<AActor>(
        ActorToSpawnCard,
        FVector(0, 0, 0),
        GetActorRotation(),
        SpawnParams
    );
    ABActorFightingCard* NewActor = dynamic_cast<ABActorFightingCard*>(NewActorRaw);
    NewActor->WhenHighlightedScaleRelative = WhenHighlightedScaleRelative;
    NewActor->WhenHighlightedLocationDelta = WhenHighlightedLocationDelta;
    NewActor->SetActorLocation(GetActorLocation());
    NewActor->ManaCost = FMana(rand() % 6, rand() % 2, rand() % 2, rand() % 2, rand() % 2);
    NewActor->ManaGives = FMana(0, rand() % 2, rand() % 2, rand() % 2, rand() % 2);;
    CardActors.Add(NewActor);

    RearrangeCardsInHand();
}

void ABActorFightingDeck::RearrangeCardsInHand()
{
    for (int i = 0; i < CardActors.Num(); ++i) {
        const auto& card = CardActors[i];
        card->MoveOverTimeTo(
            card->GetActorLocation(),
            GetActorLocation() + WHERE_IS_HAND + (i - CardActors.Num() / 2.0) * SPACE_BETWEEN_CARDS,
            DRAWING_TIME
        );
    }
}

ABActorFightingUnitBase* ABActorFightingDeck::PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    for (int i = 0; i < CardActors.Num(); ++i) {
        if (Card == CardActors[i]) {
            CardActors.RemoveAt(i);
            Card->MoveOverTimeTo(Card->GetActorLocation(), Cell->GetActorLocation(), CARD_PLAY_TIME);
            RearrangeCardsInHand();

            FTimerHandle TimerHandle;
            ABActorFightingUnitBase* NewUnit = Card->SpawnUnit(Cell, BActorFightingField->ActorToSpawnUnit);
            NewUnit->SetActorHiddenInGame(true);
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                [this, Card, Cell, NewUnit]() {
                    NewUnit->SetActorHiddenInGame(false);
                    Card->Destroy();
                },
                CARD_PLAY_TIME,
                false
            );
            return NewUnit;
        }
    }
    return nullptr;
}