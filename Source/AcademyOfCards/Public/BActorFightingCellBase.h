// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BActorEnhanced.h"
#include "BActorFightingCellBase.generated.h"

/**
 * 
 */
UCLASS()
class ACADEMYOFCARDS_API ABActorFightingCellBase : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:
	static constexpr float RADIUS = 89;

	int X = 0;
	int Y = 0;
	int Z = 0;

	static std::tuple<int, int, int, int> Reduce(int dx, int dy, int dz) {
		int adx = std::abs(dx), ady = std::abs(dy), adz = std::abs(dz);
		int dcommon = 0;
		if (dx * dy > 0 && adx <= ady) dcommon = dx;
		if (dx * dz > 0 && adx <= adz) dcommon = dx;
		if (dy * dx > 0 && ady <= adx) dcommon = dy;
		if (dy * dz > 0 && ady <= adz) dcommon = dy;
		if (dz * dx > 0 && adz <= adx) dcommon = dz;
		if (dz * dy > 0 && adz <= ady) dcommon = dz;
		return { dx - dcommon, dy - dcommon, dz - dcommon, dcommon };
	}

	static bool AreOnTheLine(TTuple<int, int, int> Cell1, TTuple<int, int, int> Cell2) {
		int dx = (Cell1.Get<0>() - Cell2.Get<0>()), dy = (Cell1.Get<1>() - Cell2.Get<1>()), dz = (Cell1.Get<2>() - Cell2.Get<2>());
		auto [ddx, ddy, ddz, ddcommon] = Reduce(dx, dy, dz);
		return ddx == ddy || ddx == ddz || ddy == ddz;
	}

	static bool AreOnTheLine(ABActorFightingCellBase* Cell1, ABActorFightingCellBase* Cell2) {
		return AreOnTheLine(Cell1->GetCoordinates(), Cell2->GetCoordinates());
	}

	static int Distance(TTuple<int, int, int> Cell1, TTuple<int, int, int> Cell2) {
		int dx = (Cell1.Get<0>() - Cell2.Get<0>()), dy = (Cell1.Get<1>() - Cell2.Get<1>()), dz = (Cell1.Get<2>() - Cell2.Get<2>());
		auto [ddx, ddy, ddz, ddcommon] = Reduce(dx, dy, dz);
		return std::abs(ddx) + std::abs(ddy) + std::abs(ddz);
	}

	static int Distance(ABActorFightingCellBase* Cell1, ABActorFightingCellBase* Cell2) {
		return Distance(Cell1->GetCoordinates(), Cell2->GetCoordinates());
	}

	TTuple<int, int, int> GetCoordinates() const { return { X, Y, Z }; }

	static bool IsValidCoordinates(TTuple<int, int, int> Coordinates, int Radius = -1) {
		if (Coordinates.Get<0>() < 0 || Coordinates.Get<1>() < 0 || Coordinates.Get<2>() < 0) return false;
		if (Coordinates.Get<0>() != 0 && Coordinates.Get<1>() != 0 && Coordinates.Get<2>() != 0) return false;
		if (Radius == -1) return true;
		if (Distance(Coordinates, { 0, 0, 0 }) >= Radius) return false;
		return true;
	}

	TArray<TTuple<int, int, int>> GetNeighboursCoordinates(int Radius = -1) const { // TODO optimize
		TArray<TTuple<int, int, int>> result;
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				for (int dz = -1; dz <= 1; ++dz) {
					if (dx == 0 && dy == 0 && dz == 0) continue;
					TTuple<int, int, int> PotentialCoordinates = { X + dx, Y + dy, Z + dz };
					if (!IsValidCoordinates(PotentialCoordinates, Radius)) continue;
					if (Distance(GetCoordinates(), PotentialCoordinates) != 1) continue;
					result.Add(PotentialCoordinates);
				}
			}
		}
		return result;
	}

	UFUNCTION(BlueprintCallable, Category = "Location")
	FVector GetUnitLocation();
};
