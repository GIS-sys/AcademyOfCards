// Fill out your copyright notice in the Description page of Project Settings.


#include "BActorFightingField.h"
#include <UMyGameInstance.h>
#include <WalkingResultFight.h>
#include <Kismet/GameplayStatics.h>
#include "BActorWalkingPlayerModel.h"
#include <any>

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
        PlayerUnitMy->InitPlayerMy(this, StartingCell, GetPlayerStats(true));
        TriggersDispatcher.AddTriggerAbilitiesFromUnit(PlayerUnitMy);
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
        PlayerUnitOpponent->InitPlayerOpponent(this, OpponentName, StartingCell, GetPlayerStats(false));
        TriggersDispatcher.AddTriggerAbilitiesFromUnit(PlayerUnitOpponent);
        ArrayUnits.Add(PlayerUnitOpponent);
    }
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
    UIManager.Init(this);
    TriggersDispatcher.Init(this);
    InitLoadFromWalking();

    InitCells();
    InitPlayers();
    InitDecks();
    InitUnits();
    PassTurn();

    UIManager.LetActionsRegular();

    OpponentStats.Health = 0;
}



void ABActorFightingField::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    for (ABActorFightingUnitBase* Unit : ArrayUnits) {
        if (Unit && !Unit->IsMovingOverTime()) Unit->MoveOverTimeTo(Unit->LocationOriginal, Unit->CurrentCell->GetUnitLocation(), 0.1);
    }

    TriggersDispatcher.Tick(DeltaTime);
}





bool ABActorFightingField::MoveUnit(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell)
{
    if (IsOccupied(Cell)) return false;
    bool res = Unit->Move(this, Cell);
    if (res) {
        TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent::MOVE, std::vector<std::any>{ Unit, Cell }));
    }
    return res;
}

bool ABActorFightingField::AttackUnit(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim)
{
    if (Attacker == Victim) return false;
    if (Attacker->UnitParameters->CurrentAttacks <= 0) return false;
    int Distance = ABActorFightingCellBase::Distance(Attacker->CurrentCell, Victim->CurrentCell);
    if (Distance > Attacker->UnitParameters->Range) return false;

    Attacker->OnAttackUnit(this, Victim);
    Victim->OnGetAttacked(this, Attacker);

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
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent::ATTACK, std::vector<std::any>{ Attacker, Victim, Attacker->UnitParameters->CurrentPower }));
    return true;
}

bool ABActorFightingField::PlayCard(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    if (ABActorFightingCellBase::Distance(GetCurrentPlayerUnit()->CurrentCell, Cell) > 1) return false;
    if (IsOccupied(Cell)) return false;
    FMana ManaRest = *GetCurrentPlayerMana() - Card->ManaCost;
    if (!ManaRest) return false;
    ABActorFightingUnitBase* NewUnit = GetCurrentPlayerDeck()->PlayCard(Card, Cell);
    if (!NewUnit) return false;
    ArrayUnits.Add(NewUnit);
    *GetCurrentPlayerMana() -= Card->ManaCost;
    *GetCurrentPlayerMana() += Card->ManaGain;
    TriggersDispatcher.AddTriggerAbilitiesFromUnit(NewUnit);
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent::PLAYCARD, std::vector<std::any>{ Card, Cell }));
    return true;
}

FString ABActorFightingField::AbilityDrawCard()
{
    if (PlayerMana.General >= 4) {
        PlayerMana.General -= 4;
        auto* NewCard = DeckMy->DrawCard();
        TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(TriggersDispatcherEvent_EnumAbility::DrawCard, std::vector<std::any>{ NewCard }));
        return "";
    }
    return "Not enough mana";
}

FString ABActorFightingField::AbilityGetMana(int& Mana, TriggersDispatcherEvent_EnumAbility ManaType)
{
    if (PlayerMana.General >= 2) {
        PlayerMana.General -= 2;
        Mana += 1;
        TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(ManaType));
        return "";
    }
    return "Not enough mana";
}

FString ABActorFightingField::AbilityGetManaLight()
{
   return AbilityGetMana(PlayerMana.Light, TriggersDispatcherEvent_EnumAbility::GetManaLight);
}

FString ABActorFightingField::AbilityGetManaDark()
{
    return AbilityGetMana(PlayerMana.Dark, TriggersDispatcherEvent_EnumAbility::GetManaDark);
}

FString ABActorFightingField::AbilityGetManaFire()
{
    return AbilityGetMana(PlayerMana.Fire, TriggersDispatcherEvent_EnumAbility::GetManaFire);
}

FString ABActorFightingField::AbilityGetManaIce()
{
    return AbilityGetMana(PlayerMana.Ice, TriggersDispatcherEvent_EnumAbility::GetManaIce);
}

FString ABActorFightingField::PassTurn()
{
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(TriggersDispatcherEvent_EnumAbility::PassTurn));
    IsPlayerTurn = !IsPlayerTurn;

    GetCurrentPlayerMana()->GeneralMax += 2;
    GetCurrentPlayerMana()->General += GetCurrentPlayerMana()->GeneralMax;
    if (GetCurrentPlayerMana()->General > GetCurrentPlayerMana()->GeneralMax) GetCurrentPlayerMana()->General = GetCurrentPlayerMana()->GeneralMax;

    auto* NewCard = GetCurrentPlayerDeck()->DrawCard();
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(TriggersDispatcherEvent_EnumEvent::DrawCardOnTurnStart, std::vector<std::any>{ NewCard }));

    for (ABActorFightingUnitBase* Unit : ArrayUnits) {
        Unit->OnTurnEnd(this, IsPlayerTurn != Unit->IsControlledByPlayer);
    }

    if (!IsPlayerTurn) {
        AIOpponent.YourTurn(this);
    }
    return "";
}



FString ABActorFightingField::ClickedOnCell(ABActorFightingCellBase* target) {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnCell(target);
}

FString ABActorFightingField::ClickedOnUnit(ABActorFightingUnitBase* target) {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnUnit(target);
}

FString ABActorFightingField::ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target) {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnAbility(target);
}

FString ABActorFightingField::ClickedOnCard(ABActorFightingCard* target) {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnCard(target);
}

FString ABActorFightingField::ClickedOnPassTurn() {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnPassTurn();
}

FString ABActorFightingField::ClickedOnOutside() {
    // cell, unit, card, buttons for abilities, button for passing turn
    // returns bool (ok/not ok) and message/error
    return UIManager.ClickedOnOutside();
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

// Implement AI logic below
void AI::InitNextTurn(ABActorFightingField* FightingField)
{
    WasThinkingFor = 0;
}

void AI::Think(ABActorFightingField* FightingField)
{
    FPlatformProcess::Sleep(0.5f);

    TTuple<int, int, int> CurrentCoordinates = FightingField->PlayerUnitOpponent->CurrentCell->GetCoordinates();
    TArray<TTuple<int, int, int>> NeighboursCoordinates = FightingField->PlayerUnitOpponent->CurrentCell->GetNeighboursCoordinates(FightingField->RADIUS);
    TArray<TTuple<int, int, int>> FreeNeighboursCoordinates;
    for (auto coordinates : NeighboursCoordinates) {
        if (!FightingField->IsOccupied(coordinates)) {
            FreeNeighboursCoordinates.Add(coordinates);
        }
    }
    
    NeedLoop = false;

    // Play card
    if (!FightingField->DeckOpponent->CardActors.IsEmpty() && !FreeNeighboursCoordinates.IsEmpty()) {
        int RandomFreeNieghbourIndex = FMath::RandRange(0, FreeNeighboursCoordinates.Num() - 1);
        PlayCardCoordinates = FreeNeighboursCoordinates[RandomFreeNieghbourIndex];
        CardToPlay = nullptr;
        for (auto* card : FightingField->DeckOpponent->CardActors) {
            if (FightingField->OpponentMana - card->ManaCost) {
                CardToPlay = card;
                FreeNeighboursCoordinates.RemoveAt(RandomFreeNieghbourIndex);
                break;
            }
        }
    } else {
        CardToPlay = nullptr;
    }

    // Move player
    if (!FreeNeighboursCoordinates.IsEmpty()) {
        MovingPlayerCoordinates = FreeNeighboursCoordinates[FMath::RandRange(0, FreeNeighboursCoordinates.Num() - 1)];
    } else {
        MovingPlayerCoordinates = { -1, -1, -1 };
    }

    // Move units
    MoveUnitCoordinates = { -1, -1, -1 };
    MoveUnit = nullptr;
    for (ABActorFightingUnitBase* unit : FightingField->ArrayUnits) {
        if (unit->IsControlledByPlayer) continue;
        if (unit->UnitParameters->CurrentMovement <= 0) continue;

        TArray<TTuple<int, int, int>> UnitNeighboursCoordinates = unit->CurrentCell->GetNeighboursCoordinates(FightingField->RADIUS);
        TArray<TTuple<int, int, int>> UnitFreeNeighboursCoordinates;
        for (auto coordinates : UnitNeighboursCoordinates) {
            if (!FightingField->IsOccupied(coordinates) && coordinates != MovingPlayerCoordinates && coordinates != PlayCardCoordinates) {
                UnitFreeNeighboursCoordinates.Add(coordinates);
            }
        }
        if (UnitFreeNeighboursCoordinates.Num() == 0) continue;

        int UnitRandomFreeNieghbourIndex = FMath::RandRange(0, UnitFreeNeighboursCoordinates.Num() - 1);
        MoveUnitCoordinates = UnitFreeNeighboursCoordinates[UnitRandomFreeNieghbourIndex];
        MoveUnit = unit;
        break;
    }

    // Attack units
    AttackUnitAttacker = nullptr;
    AttackUnitVictim = nullptr;
    if (!MoveUnit) {
        for (ABActorFightingUnitBase* unit_potential_attacker : FightingField->ArrayUnits) {
            if (unit_potential_attacker->IsControlledByPlayer) continue;
            if (unit_potential_attacker->UnitParameters->CurrentAttacks <= 0) continue;

            TArray<ABActorFightingUnitBase*> UnitVictims;
            for (ABActorFightingUnitBase* unit_potential_victim : FightingField->ArrayUnits) {
                if (!unit_potential_victim->IsControlledByPlayer) continue;
                if (ABActorFightingCellBase::Distance(unit_potential_attacker->CurrentCell, unit_potential_victim->CurrentCell) > unit_potential_attacker->UnitParameters->Range) continue;
                UnitVictims.Add(unit_potential_victim);
            }
            if (UnitVictims.Num() == 0) continue;

            AttackUnitAttacker = unit_potential_attacker;
            int UnitRandomVictimIndex = FMath::RandRange(0, UnitVictims.Num() - 1);
            AttackUnitVictim = UnitVictims[UnitRandomVictimIndex];
            break;
        }
    }
}

void AI::Act(ABActorFightingField* FightingField)
{
    // TODO
    /*if (CardToPlay) {
        int x = PlayCardCoordinates.Get<0>(); int y = PlayCardCoordinates.Get<1>(); int z = PlayCardCoordinates.Get<2>();
        UE_LOG(LogTemp, Error, TEXT("AI acting by playing card to %d %d %d"), x, y, z);
        FightingField->PlayCard(CardToPlay, FightingField->ArrayCells[x][y][z]);

        NeedLoop = true;
    }
    
    if (MovingPlayerCoordinates != TTuple<int, int, int>({ -1, -1, -1 })) {
        int x = MovingPlayerCoordinates.Get<0>(); int y = MovingPlayerCoordinates.Get<1>(); int z = MovingPlayerCoordinates.Get<2>();
        UE_LOG(LogTemp, Error, TEXT("AI acting by walking to %d %d %d"), x, y, z);
        FightingField->MoveUnit(FightingField->PlayerUnitOpponent, FightingField->ArrayCells[x][y][z]);

        NeedLoop |= (FightingField->PlayerUnitOpponent->UnitParameters->CurrentMovement != 0);
    }

    if (MoveUnit) {
        int x = MoveUnitCoordinates.Get<0>(); int y = MoveUnitCoordinates.Get<1>(); int z = MoveUnitCoordinates.Get<2>();
        UE_LOG(LogTemp, Error, TEXT("AI acting by moving unit to %d %d %d"), x, y, z);
        bool res = FightingField->MoveUnit(MoveUnit, FightingField->ArrayCells[x][y][z]);

        NeedLoop |= res;
    }

    if (AttackUnitAttacker) {
        UE_LOG(LogTemp, Error, TEXT("AI acting by attacking"));
        FightingField->AttackUnit(AttackUnitAttacker, AttackUnitVictim);
    }*/
}

bool AI::HasFinishedTurn(ABActorFightingField* FightingField)
{
    ++WasThinkingFor;
    if (WasThinkingFor > 30) return true;
    return !NeedLoop;
}