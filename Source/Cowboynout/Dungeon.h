// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cowboynout.h"
#include "GameFramework/Actor.h"
#include "Dungeon.generated.h"

UENUM(BlueprintType)
enum class ETileEnum : uint8
{
	TE_Void 	UMETA(DisplayName = "Void"),
	TE_Floor	UMETA(DisplayName = "Floor"),
	TE_Wall		UMETA(DisplayName = "Wall"),
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

	UPROPERTY()
	int positionX;

	UPROPERTY()
	int positionY;

	UPROPERTY()
	int sizeX;

	UPROPERTY()
	int sizeY;

	int GetCenterX()
	{
		return sizeX / 2 + positionX;
	}

	int GetCenterY()
	{
		return sizeY / 2 + positionY;
	}

	bool CollidesWithRoom(FRoom room)
	{
		if (positionX + sizeX < room.positionX) return false; // a is left of b
		if (positionX > room.positionX + room.sizeX) return false; // a is right of b
		if (positionY + sizeY < room.positionY) return false; // a is above b
		if (positionY > room.positionY + room.sizeY) return false; // a is below b
		return true; // boxes overlap
	}
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
	TArray<FTileStruct> tiles;

	UPROPERTY()
	TArray<FTileArrayStruct> tilesArray;

};
