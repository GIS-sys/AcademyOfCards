// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbilityGiveAbility.h"
#include "BActorFightingUnitBase.h"
#include "BActorfightingField.h"

void FightingAbilityGiveAbility::_Build() {
	if (!AdditionalArguments->TryGetStringField("ability_id", InnerAbilityID))
		if (!Arguments->TryGetStringField("ability_id", InnerAbilityID))
			InnerAbilityID = "";
}

void FightingAbilityGiveAbility::ExecEvent(ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
	Target.With(Field, Event, OwnerUnit, [&](const std::map<FString, std::any>& args, ABActorFightingField* Field, TriggersDispatcherEvent& Event, ABActorFightingUnitBase* OwnerUnit) {
		if (args.find("unit") == args.end()) {
			UE_LOG(LogTemp, Error, TEXT("FightingAbilityGiveAbility::ExecEvent need a unit"));
			// throw std::exception("FightingAbilityGiveStats needs a unit as a target");
			return;
		}
		ABActorFightingUnitBase* ChosenUnit = std::any_cast<ABActorFightingUnitBase*>(args.find("unit")->second);

		UUMyGameInstance* MyGameInstance = Cast<UUMyGameInstance>(Field->GetGameInstance());
		TSharedPtr<FightingAbility> NewAbility = MyGameInstance->LoadedFightingConfigs->GetAbilityByID(InnerAbilityID)->Build(MakeShareable(new FJsonObject()));
		ChosenUnit->UnitParameters->Abilities.Add(NewAbility);
		Field->TriggersDispatcher.AddTriggerAbility(ChosenUnit, NewAbility, true);
	});
}