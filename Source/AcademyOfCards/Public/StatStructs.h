// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSaveInstance.h"
#include "FightingAbility.h"
#include "StatStructs.generated.h"

USTRUCT(BlueprintType)
struct FUnitParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Movement = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Health = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Attacks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Range = 0;

	TArray<TSharedPtr<FightingAbility>> Abilities;
};

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Energy = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Gold = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentLight = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentDark = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentFire = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AlignmentIce = 0.0;
};

USTRUCT(BlueprintType)
struct FMana
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int General = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Light = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Dark = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Fire = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Ice = 0;

	FMana operator-(const FMana& other) const {
		return FMana{
			.General = General - other.General,
			.Light = Light - other.Light,
			.Dark = Dark - other.Dark,
			.Fire = Fire - other.Fire,
			.Ice = Ice - other.Ice,
		};
	}

	FMana operator-() const {
		return FMana() - *this;
	}

	void operator-=(const FMana& other) {
		*this += (-other);
	}

	void operator+=(const FMana& other) {
		General += other.General;
		Light += other.Light;
		Dark += other.Dark;
		Fire += other.Fire;
		Ice += other.Ice;
	}

	explicit operator bool() const {
		return General >= 0 && Light >= 0 && Dark >= 0 && Fire >= 0 && Ice >= 0;
	}
};

USTRUCT(BlueprintType)
struct FPlayerMana : public FMana
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int GeneralMax = 2;
};

UCLASS(Blueprintable, BlueprintType)
class ACADEMYOFCARDS_API UStatStructs : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Util")
	static FString ToString(const FMana& Mana) {
		return FString::Printf(TEXT("%d LD%d-%d FI%d-%d"), Mana.General, Mana.Light, Mana.Dark, Mana.Fire, Mana.Ice);
	}

	static LevelSaveInstance SavePlayerStats(const FPlayerStats& X) {
		LevelSaveInstance SaveInstance;
		SaveInstance.SetCopy("AlignmentDark", X.AlignmentDark);
		SaveInstance.SetCopy("AlignmentFire", X.AlignmentFire);
		SaveInstance.SetCopy("AlignmentIce", X.AlignmentIce);
		SaveInstance.SetCopy("AlignmentLight", X.AlignmentLight);
		SaveInstance.SetCopy("Health", X.Health);
		SaveInstance.SetCopy("Energy", X.Energy);
		SaveInstance.SetCopy("Gold", X.Gold);
		return SaveInstance;
	}

	static FPlayerStats LoadPlayerStats(LevelSaveInstance* SaveInstance) {
		FPlayerStats X;
		X.AlignmentDark = SaveInstance->GetAsCopy<float>("AlignmentDark");
		X.AlignmentFire = SaveInstance->GetAsCopy<float>("AlignmentFire");
		X.AlignmentIce = SaveInstance->GetAsCopy<float>("AlignmentIce");
		X.AlignmentLight = SaveInstance->GetAsCopy<float>("AlignmentLight");
		X.Health = SaveInstance->GetAsCopy<float>("Health");
		X.Energy = SaveInstance->GetAsCopy<int32>("Energy");
		X.Gold = SaveInstance->GetAsCopy<int32>("Gold");
		return X;
	}

	static FMana FManaConstructor(FString AlignmentKind) {
		if (AlignmentKind == "Neutral") {
			return FMana();
		} else if (AlignmentKind == "Light") {
			return FMana{ .Light = 1 };
		} else if (AlignmentKind == "Dark") {
			return FMana{ .Dark = 1 };
		} else if (AlignmentKind == "Fire") {
			return FMana{ .Fire = 1 };
		} else if (AlignmentKind == "Ice") {
			return FMana{ .Ice = 1 };
		}
		throw ("FManaConstructor got unexpected alignment kind: " + AlignmentKind);
	}

	static FMana FManaConstructor(TSharedPtr<FJsonObject> data) {
		return FMana{
			.General = (int)(data->GetNumberField("General")),
			.Light = (int)(data->GetNumberField("Light")),
			.Dark = (int)(data->GetNumberField("Dark")),
			.Fire = (int)(data->GetNumberField("Fire")),
			.Ice = (int)(data->GetNumberField("Ice"))
		};
	}

	static FUnitParameters FUnitParametersConstructor(const FUnitParameters& other) {
		FUnitParameters created;
		created.Movement = other.Movement;
		created.Health = other.Health;
		created.Power = other.Power;
		created.Attacks = other.Attacks;
		created.Range = other.Range;
		created.Abilities = other.Abilities;
		return created;
	}

	static FUnitParameters FUnitParametersConstructor(TSharedPtr<FJsonObject> data) {
		FUnitParameters created;
		if (!data->TryGetNumberField("Movement", created.Movement)) {
			created.Movement = 1;
		}
		if (!data->TryGetNumberField("Health", created.Health)) {
			created.Health = 1;
		}
		if (!data->TryGetNumberField("Power", created.Power)) {
			created.Power = 1;
		}
		if (!data->TryGetNumberField("Attacks", created.Attacks)) {
			created.Attacks = 1;
		}
		if (!data->TryGetNumberField("Range", created.Range)) {
			created.Range = 1;
		}
		return created;
	}
};