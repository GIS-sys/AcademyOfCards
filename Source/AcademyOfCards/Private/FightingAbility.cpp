// Fill out your copyright notice in the Description page of Project Settings.


#include "FightingAbility.h"
#include "FightingAbilityGiveAbility.h"
#include "FightingAbilityDealDamage.h"
#include "FightingAbilityJump.h"
#include "FightingAbilityGetStats.h"
#include "FightingAbilityGiveStats.h"
#include "BActorFightingField.h"
#include "UMyGameInstance.h"

FightingAbility::FightingAbility(TSharedPtr<FJsonObject> data, UUMyGameInstance* MyGameInstance)
{
	ID = data->GetStringField("ID");
	Type = data->GetStringField("Type");
	Description = data->GetStringField("Description");
	Arguments = data->GetObjectField("Arguments");
}

TSharedPtr<FightingAbility> FightingAbility::Build(TSharedPtr<FJsonObject> BuildArguments) const
{
	// TODOIMPORTANT
	TSharedPtr<FightingAbility> AbilityBuilt = nullptr;
	if (Type == "Jump") {
		AbilityBuilt = MakeShareable(new FightingAbilityJump());
	} else if (Type == "Get stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGetStats());
	/*} else if (Type == "Deal_damage") {
		AbilityBuilt = MakeShareable(new FightingAbilityDealDamage());
	} else if (Type == "Give ability") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveAbility());
	} else if (Type == "Give stats") {
		AbilityBuilt = MakeShareable(new FightingAbilityGiveStats());*/
	} else {
		// throw "FightingAbility::Build got unexpected Type: " + Type + " (ID: " + ID + ")"; // TODO IMPORTANT
		UE_LOG(LogTemp, Error, TEXT("ERROR UNKNOWN ABILITY TYPE %s (FightingAbility::Build)"), *Type);
		AbilityBuilt = MakeShareable(new FightingAbility());
	}

	AbilityBuilt->ID = ID;
	AbilityBuilt->Type = Type;
	AbilityBuilt->Description = Description;
	AbilityBuilt->Arguments = Arguments;
	AbilityBuilt->AdditionalArguments = BuildArguments;

	FString WhenStr;
	if (!AbilityBuilt->AdditionalArguments->TryGetStringField("when", WhenStr)) {
		if (!AbilityBuilt->Arguments->TryGetStringField("when", WhenStr)) WhenStr = "";
	}
	if (WhenStr == "Invocation") {
		AbilityBuilt->When.insert(INVOCATION);
	//} else if (WhenStr == "on_attack") {
	//	AbilityBuilt->When = ON_ATTACK;
	//} else if (WhenStr == "spell_cast") {
	//	AbilityBuilt->When = SPELL_CAST;
	} else {
		//AbilityBuilt->When = ALWAYS;
	}

	AbilityBuilt->_Build();
	return AbilityBuilt;
}