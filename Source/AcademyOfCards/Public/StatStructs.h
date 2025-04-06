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
struct FPlayerMana
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int General = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int GeneralMax = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Light = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Dark = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Fire = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Ice = 4;
};