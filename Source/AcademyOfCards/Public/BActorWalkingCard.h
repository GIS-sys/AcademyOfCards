// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BActorEnhanced.h"
#include "LevelSaveInstance.h"
#include "BActorWalkingCard.generated.h"
class WalkingCardConfig;
class ABActorWalkingDealer;

UCLASS()
class ACADEMYOFCARDS_API ABActorWalkingCard : public ABActorEnhanced
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABActorWalkingCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsCloseUpLook = false;

public:
	struct WallsStruct {
		bool left;
		bool right;
		bool top;
		bool bottom;

		bool IsAllowedMovement(int dx, int dy) {
			if (dx == -1 && dy == 0) return !left;
			if (dx == 1 && dy == 0) return !right;
			if (dx == 0 && dy == -1) return !top;
			if (dx == 0 && dy == 1) return !bottom;
			// if movement is not single step - return false by default
			return false;
		}

		void Destroy(int dx, int dy, bool wall_value = false) {
			if (dx == -1 && dy == 0) left = wall_value;
			if (dx == 1 && dy == 0) right = wall_value;
			if (dx == 0 && dy == -1) top = wall_value;
			if (dx == 0 && dy == 1) bottom = wall_value;
		}
	};
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	int MoveTo(); // returns 0 if move is unavailable, 1 if moved and discovered new card, 2 if moved on already discovered card

	UFUNCTION(BlueprintCallable)
	bool GetWallLeft() const { return Walls.left; };
	UFUNCTION(BlueprintCallable)
	bool GetWallRight() const { return Walls.right; };
	UFUNCTION(BlueprintCallable)
	bool GetWallTop() const { return Walls.top; };
	UFUNCTION(BlueprintCallable)
	bool GetWallBottom() const { return Walls.bottom; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* MainCardMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int BoardPositionX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	int BoardPositionY;

	ABActorWalkingDealer* DealerPtr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	bool IsDiscovered = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	bool IsOpenlyVisible = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	bool IsMovingCloseUpLook = false;

	UFUNCTION(BlueprintCallable, Category = "Actor")
	bool GetCloseUpLook() const { return IsCloseUpLook; }

	UFUNCTION(BlueprintCallable, Category = "Actor")
	void SetCloseUpLook(bool NewCloseUpLook) { IsCloseUpLook = NewCloseUpLook; IsMovingCloseUpLook = true; }

	UFUNCTION(BlueprintCallable)
	bool IsCollectible();

	WallsStruct Walls;

	TSharedPtr<WalkingCardConfig> CardConfig;

	LevelSaveInstance Save();
	void Load(LevelSaveInstance* SaveInstance);
};
