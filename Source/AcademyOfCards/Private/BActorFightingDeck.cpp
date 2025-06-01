// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingDeck.h"
#include <BActorFightingField.h>
#include <UMyGameInstance.h>

void ABActorFightingDeck::BeginPlay()
{
    Super::BeginPlay();
    MyGameInstance = Cast<UUMyGameInstance>(GetGameInstance());
}

void ABActorFightingDeck::DealCards()
{
    for (int i = 0; i < STARTING_HAND; ++i) {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this]() { DrawCard(); },
            DRAWING_TIME * i + 0.001,
            false
        );
    }
}

ABActorFightingCard* ABActorFightingDeck::DrawCard()
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

    FString RandomCardId = GetRandomCardId();
    NewActor->FromConfig(MyGameInstance->LoadedFightingConfigs->GetCardByID(RandomCardId));

    // set material
    int i = 0;
    if (BActorFightingField->MaterialCardIDsArray.Find(NewActor->ID, i)) {
        NewActor->MainMaterial = BActorFightingField->MaterialCardArray[i];
        NewActor->UnitMaterial = BActorFightingField->MaterialUnitArray[i];
    } else {
        NewActor->MainMaterial = BActorFightingField->MaterialCardArray[0];
        NewActor->UnitMaterial = BActorFightingField->MaterialUnitArray[0];
    }

    NewActor->IsControlledByPlayer = IsControlledByPlayer;
    CardActors.Add(NewActor);

    RearrangeCardsInHand();
    return NewActor;
}

void VectorClamp(FVector& VOriginal, const FVector& VMin, const FVector& VMax)
{
    for (int i = 0; i < 3; ++i) {
        if (VOriginal[i] < VMin[i]) VOriginal[i] = VMin[i];
        if (VOriginal[i] > VMax[i]) VOriginal[i] = VMax[i];
    }
}

void ABActorFightingDeck::RearrangeCardsInHand()
{
    FVector SpaceBetweenCards = MAX_HAND_SIZE / CardActors.Num();
    VectorClamp(SpaceBetweenCards, MIN_SPACE_BETWEEN_CARDS, MAX_SPACE_BETWEEN_CARDS);
    for (int i = 0; i < CardActors.Num(); ++i) {
        const auto& card = CardActors[i];
        card->MoveOverTimeTo(
            card->GetActorLocation(),
            GetActorLocation() + WHERE_IS_HAND + (i - CardActors.Num() / 2.0) * SpaceBetweenCards,
            DRAWING_TIME
        );
    }
}

ABActorFightingUnitBase* ABActorFightingDeck::PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    for (int i = 0; i < CardActors.Num(); ++i) {
        if (Card == CardActors[i]) {
            CardActors.RemoveAt(i);
            Card->IsBeingPlayed = true;
            Card->MoveOverTimeTo(Card->GetActorLocation(), Cell->GetActorLocation(), CARD_PLAY_TIME);
            RearrangeCardsInHand();

            FTimerHandle TimerHandle;
            ABActorFightingUnitBase* NewUnit = Card->SpawnUnit(BActorFightingField, Cell, BActorFightingField->ActorToSpawnUnit);
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