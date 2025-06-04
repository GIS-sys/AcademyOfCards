// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingCard.h"
#include <UMyGameInstance.h>
#include "StatStructs.h"

FightingCard::FightingCard(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
    ID = data->GetStringField("ID");
    Name = data->GetStringField("Name");
    Cardtext = data->GetStringField("Cardtext");
    Type = data->GetStringField("Type");
    AlignmentKind = data->GetStringField("AlignmentKind");
    Rarity = data->GetStringField("Rarity");
    ManaCost = UStatStructs::FManaConstructor(data->GetObjectField("ManaCost"));
    UnitParameters = UStatStructs::FUnitParametersConstructor(data->GetObjectField("UnitParameters"));

    for (const auto& ability : data->GetArrayField("Abilities")) {
        const auto& AbilityObject = ability->AsObject();
        FString AbilityID = AbilityObject->GetStringField("ID");
        TSharedPtr<FJsonObject> AbilityArguments = AbilityObject->GetObjectField("Arguments");
        UnitParameters.Abilities.Add(MyGameInstance->LoadedFightingConfigs->GetAbilityByID(AbilityID)->Build(AbilityArguments));
    }
    if (Type == UStatStructs::TYPE_SPELL()) {
        UnitParameters.Abilities.Add(FightingAbility::FactoryBuildSpellDeath());
    }
}

FightingCard::~FightingCard()
{
}
