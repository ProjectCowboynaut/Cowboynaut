// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Dungeon.generated.h"

UENUM(BlueprintType)
enum class ETileEnum : uint8
{
	TE_Void 	UMETA(DisplayName = "Void"),
	TE_Floor	UMETA(DisplayName = "Floor"),
	TE_WallLL	UMETA(DisplayName = "WallLL"),
	TE_WallUU	UMETA(DisplayName = "WallUU"),
	TE_WallRR	UMETA(DisplayName = "WallRR"),
	TE_WallDD	UMETA(DisplayName = "WallDD"),
	TE_WallUL	UMETA(DisplayName = "WallUL"),
	TE_WallUR	UMETA(DisplayName = "WallUR"),
	TE_WallDL	UMETA(DisplayName = "WallDL"),
	TE_WallDR	UMETA(DisplayName = "WallDR"),
	TE_Max		UMETA(DisplayName = "Max (dont use)")
};

USTRUCT(BlueprintType)
struct FTileStruct
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly)
		ETileEnum type;

	UPROPERTY(EditDefaultsOnly)
		UStaticMesh* mesh;

	UPROPERTY()
		UInstancedStaticMeshComponent* iscm;

};

USTRUCT()
struct FTileArrayStruct
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<UInstancedStaticMeshComponent*> tiles;
};

USTRUCT(BlueprintType)
struct FRoom
{
	GENERATED_BODY()

		FRoom()
	{
		location = FVector2D(0, 0);
		extends = FVector2D(1, 1);
	}

	FRoom(FVector2D loc, FVector2D ext)
	{
		location = loc;
		extends = ext;
	}

	FVector2D getMin()
	{
		return FVector2D(location.X - extends.X, location.Y - extends.Y);
	}

	FVector2D getMax()
	{
		return FVector2D(location.X + extends.X, location.Y + extends.Y);
	}

	FVector2D getSize()
	{
		return FVector2D(2 * extends.X + 1, 2 * extends.Y + 1);
	}

	FRoom minkowskiDifference(FRoom other)
	{
		FVector2D topLeft = getMin() - other.getMin();
		FVector2D fullSize = getSize() + other.getSize();
		return FRoom(topLeft + (fullSize / 2), fullSize / 2);
	}

	bool isCollidingWith(FRoom other)
	{
		return (FMath::Abs(getMin().X - 1 - other.getMin().X - 1) * 2 < (getSize().X + 1 + other.getSize().X + 1) &&
			FMath::Abs(getMin().Y - 1 - other.getMin().Y - 1) * 2 < (getSize().Y + 1 + other.getSize().Y + 1));
		/*
		FRoom md = minkowskiDifference(other);
		return (md.getMin().Y <= 0 &&
		md.getMax().X >= 0 &&
		md.getMin().Y <= 0 &&
		md.getMax().Y >= 0);
		*/
	}

	FVector2D closestPointOnBoundsToPoint(FVector2D point)
	{
		float minDist = FMath::Abs(point.X - getMin().X);
		FVector2D boundsPoint = FVector2D(getMin().X, point.Y);
		if (FMath::Abs(getMax().X - point.X) < minDist)
		{
			minDist = FMath::Abs(getMax().X - point.X);
			boundsPoint = FVector2D(getMax().X, point.Y);
		}
		if (FMath::Abs(getMax().Y - point.Y) < minDist)
		{
			minDist = FMath::Abs(getMax().Y - point.Y);
			boundsPoint = FVector2D(point.X, getMax().Y);
		}
		if (FMath::Abs(getMin().Y - point.Y) < minDist)
		{
			minDist = FMath::Abs(getMin().Y - point.Y);
			boundsPoint = FVector2D(point.X, getMin().Y);
		}
		return boundsPoint;
	}

	FVector2D getPenetrationVector(FRoom other)
	{
		FRoom minkowski = minkowskiDifference(other);
		return minkowski.closestPointOnBoundsToPoint(FVector2D::ZeroVector);
	}

	UPROPERTY()
		FVector2D location;

	UPROPERTY()
		FVector2D extends;
};

UCLASS()
class COWBOYNOUT_API ADungeon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADungeon();

	/*
	UPROPERTY()
	TMap<ETileEnum, TArray<uint16>> tileAddressMap;
	*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		float tileSize;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		int gridSize;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		int roomSize;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		int amountEnemies;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		TArray<FTileStruct> tiles;

	UPROPERTY()
		TArray<FTileArrayStruct> tilesArray;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		TSubclassOf<class AEnemy> enemyBP;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		TSubclassOf<class AActor> TPEnter;

	UPROPERTY(EditDefaultsOnly, Category = Level)
		TSubclassOf<class AActor> TPExit;

	static FVector2D getRandomPointInCircle(float radius);

};
