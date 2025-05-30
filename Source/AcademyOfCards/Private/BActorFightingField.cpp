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
    PassTurnWithEvent(false);

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





FString ABActorFightingField::MoveUnitWithEvent(ABActorFightingUnitBase* Unit, ABActorFightingCellBase* Cell)
{
    bool res = Unit->CanMove(this, Cell);
    if (!res) return "Can't move";

    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
        TriggersDispatcherEvent_EnumEvent::MOVE,
        std::map<FString, std::any>{ {"unit", Unit}, { "from_cell", Unit->CurrentCell }, { "cell", Cell }, { "move_cost", ABActorFightingCellBase::Distance(Cell, Unit->CurrentCell) }, {"proceed", true}, {"result", FString("")} },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            ABActorFightingUnitBase* PrUnit = std::any_cast<ABActorFightingUnitBase*>(results["unit"]);
            ABActorFightingCellBase* PrCell = std::any_cast<ABActorFightingCellBase*>(results["cell"]);
            ABActorFightingCellBase* PrFromCell = std::any_cast<ABActorFightingCellBase*>(results["from_cell"]);
            int PrMoveCost = std::any_cast<int>(results["move_cost"]);
            PrUnit->Move(this, PrCell, PrMoveCost);
            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                TriggersDispatcherEvent_EnumEvent::MOVED,
                std::map<FString, std::any>{ {"unit", PrUnit}, { "from_cell", PrFromCell }, { "cell", PrCell } }
            ));
            return FString();
        }
    ));
    return "";
}

FString ABActorFightingField::DealDamageWithEvent(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim, int Damage) {
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
        TriggersDispatcherEvent_EnumEvent::TAKE_DAMAGE,
        std::map<FString, std::any>{ {"unit", Victim}, { "source_unit", Attacker }, { "damage", Damage }, { "proceed", true }, { "result", FString("") } },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            ABActorFightingUnitBase* PrVictim = std::any_cast<ABActorFightingUnitBase*>(results["unit"]);
            ABActorFightingUnitBase* PrSourceUnit = std::any_cast<ABActorFightingUnitBase*>(results["source_unit"]);
            int PrDamage = std::any_cast<int>(results["damage"]);
            PrVictim->TakeDamage(PrDamage);
            if (PrVictim->IsPlayer) {
                PrVictim->UnitParameters->Health = PrVictim->UnitParameters->CurrentHealth;
                GetPlayerStats(PrVictim->IsControlledByPlayer)->Health = PrVictim->UnitParameters->Health;
            }

            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                TriggersDispatcherEvent_EnumEvent::TOOK_DAMAGE,
                std::map<FString, std::any>{ {"unit", PrVictim}, { "source_unit", PrSourceUnit }, { "damage", PrDamage } },
                [this](std::map<FString, std::any> results) {
                    // Act
                    ABActorFightingUnitBase* PrVictim = std::any_cast<ABActorFightingUnitBase*>(results["unit"]);
                    ABActorFightingUnitBase* PrSourceUnit = std::any_cast<ABActorFightingUnitBase*>(results["source_unit"]);
                    int PrDamage = std::any_cast<int>(results["damage"]);
                    // Check if need to die
                    bool Result2 = PrVictim->IsDead(this);
                    if (!Result2) return FString("Doesn't die");

                    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                        TriggersDispatcherEvent_EnumEvent::UNIT_DYING,
                        std::map<FString, std::any>{ {"unit", PrVictim}, { "source_unit", PrSourceUnit }, { "proceed", true }, { "result", FString("") } },
                        [this](std::map<FString, std::any> results) {
                            // If we were told to stop - stop
                            FString Result = std::any_cast<FString>(results["result"]);
                            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
                            // Act
                            ABActorFightingUnitBase* PrVictim = std::any_cast<ABActorFightingUnitBase*>(results["unit"]);
                            ABActorFightingUnitBase* PrSourceUnit = std::any_cast<ABActorFightingUnitBase*>(results["source_unit"]);
                            FString Result2 = DeleteUnit(PrVictim);
                            if (!Result2.IsEmpty()) return Result2;

                            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                                TriggersDispatcherEvent_EnumEvent::UNIT_DIED,
                                std::map<FString, std::any>{ {"unit", PrVictim}, { "source_unit", PrSourceUnit } }
                            ));
                            return FString();
                        }
                    ));
                    return FString();
                }
            ));
            return FString();
        }
    ));
    return FString();
}

FString ABActorFightingField::AttackUnitWithEvent(ABActorFightingUnitBase* Attacker, ABActorFightingUnitBase* Victim)
{
    bool res = Attacker->CanAttack(this, Victim);
    if (!res) return "Can't attack";

    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
        TriggersDispatcherEvent_EnumEvent::ATTACK,
        std::map<FString, std::any>{ {"attacker", Attacker}, {"victim", Victim}, {"attacks", 1}, {"damage", Attacker->UnitParameters->CurrentPower}, {"proceed", true}, {"result", FString("")} },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            ABActorFightingUnitBase* PrAttacker = std::any_cast<ABActorFightingUnitBase*>(results["attacker"]);
            ABActorFightingUnitBase* PrVictim = std::any_cast<ABActorFightingUnitBase*>(results["victim"]);
            int PrAttacksSpent = std::any_cast<int>(results["attacks"]);
            int PrDamage = std::any_cast<int>(results["damage"]);
            PrAttacker->UnitParameters->CurrentAttacks -= PrAttacksSpent;


            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                TriggersDispatcherEvent_EnumEvent::ATTACKED,
                std::map<FString, std::any>{ {"attacker", PrAttacker}, { "victim", PrVictim }, { "attacks", PrAttacksSpent }, { "damage", PrDamage } },
                [this](std::map<FString, std::any> results) {
                    // Act
                    ABActorFightingUnitBase* PrAttacker = std::any_cast<ABActorFightingUnitBase*>(results["attacker"]);
                    ABActorFightingUnitBase* PrVictim = std::any_cast<ABActorFightingUnitBase*>(results["victim"]);
                    int PrAttacksSpent = std::any_cast<int>(results["attacks"]);
                    int PrDamage = std::any_cast<int>(results["damage"]);

                    return DealDamageWithEvent(PrAttacker, PrVictim, PrDamage);
                }
            ));
            return FString();
        }
    ));
    return "";
}

FString ABActorFightingField::DeleteUnit(ABActorFightingUnitBase* Unit) {
    for (int i = 0; i < ArrayUnits.Num(); ++i) {
        if (ArrayUnits[i] == Unit) ArrayUnits.RemoveAt(i);
    }
    for (auto& ability : Unit->UnitParameters->Abilities) {
        this->TriggersDispatcher.DeleteTriggerAbility(Unit, ability);
    }
    if (!Unit->IsPlayer) {
        Unit->Destroy();
    }
    else {
        IsFinished = true;
        IsPlayerWinner = (Unit != PlayerUnitMy);
    }
    return "";
}
FString ABActorFightingField::PlayCardWithEvent(ABActorFightingCard* Card, ABActorFightingCellBase* Cell)
{
    bool res = Card->CanBePlayed(this, Cell);
    if (!res) return "Can't play the card on this Cell";

    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
        TriggersDispatcherEvent_EnumEvent::PLAY_CARD,
        std::map<FString, std::any>{ {"card", Card}, { "cell", Cell }, { "mana_cost", Card->ManaCost }, { "mana_gain", Card->ManaGain }, {"proceed", true}, {"result", FString("")} },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            ABActorFightingCard* PrCard = std::any_cast<ABActorFightingCard*>(results["card"]);
            ABActorFightingCellBase* PrCell = std::any_cast<ABActorFightingCellBase*>(results["cell"]);
            FMana PrManaCost = std::any_cast<FMana>(results["mana_cost"]);
            FMana PrManaGain = std::any_cast<FMana>(results["mana_gain"]);
            
            ABActorFightingUnitBase* NewUnit = GetCurrentPlayerDeck()->PlayCard(PrCard, PrCell);
            if (!NewUnit) return FString("Couldn't play card");
            ArrayUnits.Add(NewUnit);
            *GetCurrentPlayerMana() -= PrManaCost;
            *GetCurrentPlayerMana() += PrManaGain;
            TriggersDispatcher.AddTriggerAbilitiesFromUnit(NewUnit);
            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                TriggersDispatcherEvent_EnumEvent::PLAYED_CARD,
                std::map<FString, std::any>{ {"card", PrCard}, { "cell", PrCell }, { "mana_cost", PrManaCost }, { "mana_gain", PrManaGain }, { "created_unit", NewUnit } }
            ));
            return FString("");
        }
    ));
    return "";
}



FString ABActorFightingField::AbilityDrawCardWithEvent()
{
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
        TriggersDispatcherEvent_EnumAbility::DRAW_CARD,
        std::map<FString, std::any>{ { "cost", 4 }, {"proceed", true}, {"result", FString("")} },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            int PrCost = std::any_cast<int>(results["cost"]);
            if (PlayerMana.General < PrCost) return FString("Not enough mana");
            PlayerMana.General -= PrCost;

            ABActorFightingCard* NewCard = DeckMy->DrawCard();
            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
                TriggersDispatcherEvent_EnumAbility::DREW_CARD,
                std::map<FString, std::any>{ {"card", NewCard}, {"cost", PrCost } }
            ));
            return FString();
        }
    ));
    return "";
}

FString ABActorFightingField::PassTurnWithEvent(bool DoEvent)
{
    if (DoEvent) {
        TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
            TriggersDispatcherEvent_EnumAbility::PASS_TURN,
            std::map<FString, std::any>{ { "mana_gain", 2 }, {"proceed", true}, {"result", FString("")} },
            [this](std::map<FString, std::any> results) {
                // If we were told to stop - stop
                FString Result = std::any_cast<FString>(results["result"]);
                if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
                // Act
                int PrManaGain = std::any_cast<int>(results["mana_gain"]);

                IsPlayerTurn = !IsPlayerTurn;
                GetCurrentPlayerMana()->GeneralMax += PrManaGain;
                GetCurrentPlayerMana()->General += GetCurrentPlayerMana()->GeneralMax;
                if (GetCurrentPlayerMana()->General > GetCurrentPlayerMana()->GeneralMax) GetCurrentPlayerMana()->General = GetCurrentPlayerMana()->GeneralMax;

                TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
                    TriggersDispatcherEvent_EnumAbility::PASSED_TURN,
                    std::map<FString, std::any>{ {"mana_gain", PrManaGain}, { "is_player_turn", IsPlayerTurn }, { "proceed", true }, { "result", FString("") } },
                    [this](std::map<FString, std::any> results) {
                        // If we were told to stop - stop
                        FString Result = std::any_cast<FString>(results["result"]);
                        if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
                        // Act
                        int PrManaGain = std::any_cast<int>(results["mana_gain"]);
                        bool PrIsPlayerTurn = std::any_cast<bool>(results["is_player_turn"]);

                        for (auto& Unit : ArrayUnits)
                            Unit->ResetOnTurnEnd(IsPlayerTurn != Unit->IsControlledByPlayer);

                        TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                            TriggersDispatcherEvent_EnumEvent::DRAW_CARD_ON_TURN_START,
                            std::map<FString, std::any>{ {"mana_gain", PrManaGain}, { "is_player_turn", PrIsPlayerTurn }, { "proceed", true }, { "result", FString("") } },
                            [this](std::map<FString, std::any> results) {
                                // If we were told to stop - stop
                                FString Result = std::any_cast<FString>(results["result"]);
                                if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
                                // Act
                                int PrManaGain = std::any_cast<int>(results["mana_gain"]);
                                bool PrIsPlayerTurn = std::any_cast<bool>(results["is_player_turn"]);

                                auto* NewCard = GetCurrentPlayerDeck()->DrawCard();
                                TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeEvent(
                                    TriggersDispatcherEvent_EnumEvent::DREW_CARD_ON_TURN_START,
                                    std::map<FString, std::any>{ {"mana_gain", PrManaGain}, { "is_player_turn", PrIsPlayerTurn }, { "card", NewCard }, {"proceed", true}, {"result", FString("")} },
                                    [this](std::map<FString, std::any> results) {
                                        // If we were told to stop - stop
                                        FString Result = std::any_cast<FString>(results["result"]);
                                        if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
                                        // Act
                                        int PrManaGain = std::any_cast<int>(results["mana_gain"]);
                                        bool PrIsPlayerTurn = std::any_cast<bool>(results["is_player_turn"]);
                                        ABActorFightingCard* PrCard = std::any_cast<ABActorFightingCard*>(results["card"]);

                                        if (!IsPlayerTurn) {
                                            AIOpponent.YourTurn(this);
                                        }
                                        return FString();
                                    }
                                ));
                                return FString();
                            }
                        ));
                        return FString();
                    }
                ));
                return FString();
            }
        ));
        return "";
    } else {
        IsPlayerTurn = !IsPlayerTurn;
        GetCurrentPlayerMana()->GeneralMax += 2;
        GetCurrentPlayerMana()->General += GetCurrentPlayerMana()->GeneralMax;
        if (GetCurrentPlayerMana()->General > GetCurrentPlayerMana()->GeneralMax) GetCurrentPlayerMana()->General = GetCurrentPlayerMana()->GeneralMax;

        auto* NewCard = GetCurrentPlayerDeck()->DrawCard();

        if (!IsPlayerTurn) {
            AIOpponent.YourTurn(this);
        }
        return "";
    }
}

FString ABActorFightingField::AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility ManaType)
{
    TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
        ManaType,
        std::map<FString, std::any>{ { "mana_type", ManaType }, { "cost", 2 }, { "amount", 1 }, {"proceed", true}, {"result", FString("")} },
        [this](std::map<FString, std::any> results) {
            // If we were told to stop - stop
            FString Result = std::any_cast<FString>(results["result"]);
            if (results.find("proceed") == results.end() || !std::any_cast<bool>(results["proceed"])) return Result;
            // Act
            TriggersDispatcherEvent_EnumAbility PrManaType = std::any_cast<TriggersDispatcherEvent_EnumAbility>(results["mana_type"]);
            int PrCost = std::any_cast<int>(results["cost"]);
            int PrAmount = std::any_cast<int>(results["amount"]);
            if (PlayerMana.General < PrCost) return FString("Not enough mana");
            
            PlayerMana.General -= PrCost;
            TriggersDispatcherEvent_EnumAbility PrManaTypePastevent = TriggersDispatcherEvent_EnumAbility::NONE;
            if (PrManaType == TriggersDispatcherEvent_EnumAbility::GET_MANA_DARK) {
                GetCurrentPlayerMana()->Dark += PrAmount;
                PrManaTypePastevent = TriggersDispatcherEvent_EnumAbility::GOT_MANA_DARK;
            } else if (PrManaType == TriggersDispatcherEvent_EnumAbility::GET_MANA_FIRE) {
                GetCurrentPlayerMana()->Fire += PrAmount;
                PrManaTypePastevent = TriggersDispatcherEvent_EnumAbility::GOT_MANA_FIRE;
            } else if (PrManaType == TriggersDispatcherEvent_EnumAbility::GET_MANA_ICE) {
                GetCurrentPlayerMana()->Ice += PrAmount;
                PrManaTypePastevent = TriggersDispatcherEvent_EnumAbility::GOT_MANA_ICE;
            } else if (PrManaType == TriggersDispatcherEvent_EnumAbility::GET_MANA_LIGHT) {
                GetCurrentPlayerMana()->Light += PrAmount;
                PrManaTypePastevent = TriggersDispatcherEvent_EnumAbility::GOT_MANA_LIGHT;
            }

            TriggersDispatcher.AddEvent(TriggersDispatcherEvent::MakeAbility(
                PrManaTypePastevent,
                std::map<FString, std::any>{ { "mana_type", PrManaType }, { "cost", PrCost }, { "amount", PrAmount } }
            ));
            return FString();
        }
    ));
    return "";
}

FString ABActorFightingField::AbilityGetManaLightWithEvent()
{
    return AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility::GET_MANA_LIGHT);
}

FString ABActorFightingField::AbilityGetManaDarkWithEvent()
{
    return AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility::GET_MANA_DARK);
}

FString ABActorFightingField::AbilityGetManaFireWithEvent()
{
    return AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility::GET_MANA_FIRE);
}

FString ABActorFightingField::AbilityGetManaIceWithEvent()
{
    return AbilityGetManaWithEvent(TriggersDispatcherEvent_EnumAbility::GET_MANA_ICE);
}



// cell, unit, card, buttons for abilities, button for passing turn, outside of everything previous
// return bool (ok/not ok) and message/error
FString ABActorFightingField::ClickedOnCell(ABActorFightingCellBase* target) {
    return UIManager.ClickedOnCell(target);
}

FString ABActorFightingField::ClickedOnUnit(ABActorFightingUnitBase* target) {
    return UIManager.ClickedOnUnit(target);
}

FString ABActorFightingField::ClickedOnAbility(TriggersDispatcherEvent_EnumAbility target) {
    return UIManager.ClickedOnAbility(target);
}

FString ABActorFightingField::ClickedOnCard(ABActorFightingCard* target) {
    return UIManager.ClickedOnCard(target);
}

FString ABActorFightingField::ClickedOnPassTurn() {
    return UIManager.ClickedOnPassTurn();
}

FString ABActorFightingField::ClickedOnOutside() {
    return UIManager.ClickedOnOutside();
}



bool ABActorFightingField::CanClickOnCell() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnCell);  }

bool ABActorFightingField::CanClickOnUnit() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnUnit); }

bool ABActorFightingField::CanClickOnAbility() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnAbility); }

bool ABActorFightingField::CanClickOnCard() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnCard); }

bool ABActorFightingField::CanClickOnPassTurn() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnPassTurn); }

bool ABActorFightingField::CanClickOnOutside() { return UIManager.CheckIfCallbackExists(FightingUIManagerClickType::OnOutside); }





void AI::StartThinkingLoop(ABActorFightingField* FightingField) {
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, FightingField]() {
        this->Think(FightingField);

        AsyncTask(ENamedThreads::GameThread, [this, FightingField]() {
            this->Act(FightingField);
            if (this->HasFinishedTurn(FightingField)) {
                FightingField->PassTurnWithEvent(true);
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
        UE_LOG(LogTemp, Warning, TEXT("AI acting by playing card to %d %d %d"), x, y, z);
        FightingField->PlayCard(CardToPlay, FightingField->ArrayCells[x][y][z]);

        NeedLoop = true;
    }
    
    if (MovingPlayerCoordinates != TTuple<int, int, int>({ -1, -1, -1 })) {
        int x = MovingPlayerCoordinates.Get<0>(); int y = MovingPlayerCoordinates.Get<1>(); int z = MovingPlayerCoordinates.Get<2>();
        UE_LOG(LogTemp, Warning, TEXT("AI acting by walking to %d %d %d"), x, y, z);
        FightingField->MoveUnit(FightingField->PlayerUnitOpponent, FightingField->ArrayCells[x][y][z]);

        NeedLoop |= (FightingField->PlayerUnitOpponent->UnitParameters->CurrentMovement != 0);
    }

    if (MoveUnit) {
        int x = MoveUnitCoordinates.Get<0>(); int y = MoveUnitCoordinates.Get<1>(); int z = MoveUnitCoordinates.Get<2>();
        UE_LOG(LogTemp, Warning, TEXT("AI acting by moving unit to %d %d %d"), x, y, z);
        bool res = FightingField->MoveUnit(MoveUnit, FightingField->ArrayCells[x][y][z]);

        NeedLoop |= res;
    }

    if (AttackUnitAttacker) {
        UE_LOG(LogTemp, Warning, TEXT("AI acting by attacking"));
        FightingField->AttackUnit(AttackUnitAttacker, AttackUnitVictim);
    }*/
}

bool AI::HasFinishedTurn(ABActorFightingField* FightingField)
{
    ++WasThinkingFor;
    if (WasThinkingFor > 30) return true;
    return !NeedLoop;
}