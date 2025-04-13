// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingCard.h"
#include <UMyGameInstance.h>

FightingCard::FightingCard(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
	UnitParameters = NewObject<UFightingUnitParameters>(MyGameInstance, UFightingUnitParameters::StaticClass());

    ID = data->GetStringField("ID");
    Name = data->GetStringField("Name");
    Cardtext = data->GetStringField("Cardtext");
    Type = data->GetStringField("Type");
    AlignmentKind = data->GetStringField("AlignmentKind");
    ManaCost = UStatStructs::FManaConstructor(data->GetObjectField("ManaCost"));
    UnitParameters->FromJsonObject(data->GetObjectField("UnitParameters"));
    for (const auto& ability : data->GetArrayField("Abilities")) {
        const auto& AbilityObject = ability->AsObject();
        FString AbilityID = AbilityObject->GetStringField("ID");
        TSharedPtr<FJsonObject> AbilityArguments = AbilityObject->GetObjectField("Arguments");
        UnitParameters->Abilities.Add(MyGameInstance->LoadedFightingConfigs->GetAbilityByID(AbilityID)->Build(AbilityArguments));
    }
}

FightingCard::~FightingCard()
{
}
