// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatStructs.generated.h"

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

	void operator-=(const FMana& other) {
		General -= other.General;
		Light -= other.Light;
		Dark -= other.Dark;
		Fire -= other.Fire;
		Ice -= other.Ice;
	}

	explicit operator bool() const {
		return General >= 0 && Light >= 0 && Dark >= 0 && Fire >= 0 && Ice >= 0;
	}

	/*bool BiggerThanZero() const {
		return General > 0 && Light > 0 && Dark > 0 && Fire > 0 && Ice > 0;
	}*/
};

USTRUCT(BlueprintType)
struct FPlayerMana : public FMana
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int GeneralMax = 2;
};