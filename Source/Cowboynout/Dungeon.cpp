// Fill out your copyright notice in the Description page of Project Settings.

#include "Dungeon.h"
#include "CowboynoutCharacter.h"
#include <vector>
#include <cmath>

// Sets default values
ADungeon::ADungeon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	tileSize = 400.f;
	gridSize = 16;
	roomSize = 5;

}

// Called when the game starts or when spawned
void ADungeon::BeginPlay()
{
	Super::BeginPlay();


	std::vector<std::vector<uint8>> matrix;
	matrix.resize(gridSize, std::vector<uint8>(gridSize, 0));




	// Initialize all Meshes with ISMC, SM and Enum Type
	for (int32 Index = 0; Index != tiles.Num(); ++Index)
	{
		tiles[Index].iscm = NewObject<UInstancedStaticMeshComponent>(this);
		tiles[Index].iscm->RegisterComponent();
		tiles[Index].iscm->SetFlags(RF_Transactional);
		if (tiles[Index].mesh) { tiles[Index].iscm->SetStaticMesh(tiles[Index].mesh); }
		this->AddInstanceComponent(tiles[Index].iscm);
	}

	// Split array by type
	for (uint8 i = 0; i < (uint8)ETileEnum::TE_Max; ++i)
	{
		for (int32 j = 0; j != tiles.Num(); ++j)
		{
			if ((uint8)tiles[j].type == i)
			{
				if (!tilesArray.IsValidIndex(i))
				{
					tilesArray.Add(FTileArrayStruct());
				}
				if (tilesArray.IsValidIndex(i))
				{
					tilesArray[i].tiles.Add(tiles[j].iscm);
				}
			}
		}
	}

	/*
	for (int32 i = 0; i < gridSize; ++i)
	{
	for (int32 j = 0; j < gridSize; ++j)
	{
	matrix[i][j] = (uint8)ETileEnum::TE_Void;//FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
	}
	}
	*/

	//---------------------------------------------------------------------------------------

	FRoom rooms[3];

	//FRoom room[0];
	rooms[0].positionX = FMath::RandRange(1, gridSize - roomSize - 2);
	rooms[0].positionY = FMath::RandRange(1, gridSize - roomSize - 2);
	rooms[0].sizeX = roomSize;
	rooms[0].sizeY = roomSize;
	for (int32 i = 0; i < roomSize; ++i)
	{
		for (int32 j = 0; j < roomSize; ++j)
		{
			matrix[i + rooms[0].positionX][j + rooms[0].positionY] = (uint8)ETileEnum::TE_Floor;//FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
		}
	}

	bool roomFound = false;
	int zahl = 0;
	//FRoom rooms[1];
	while (!(roomFound || zahl > 1000))
	{
		rooms[1].positionX = FMath::RandRange(1, gridSize - roomSize - 2);
		rooms[1].positionY = FMath::RandRange(1, gridSize - roomSize - 2);
		rooms[1].sizeX = roomSize;
		rooms[1].sizeY = roomSize;
		roomFound = !rooms[0].CollidesWithRoom(rooms[1]);
		zahl++;

		if (roomFound)
		{
			for (int32 i = 0; i < roomSize; ++i)
			{
				for (int32 j = 0; j < roomSize; ++j)
				{
					matrix[i + rooms[1].positionX][j + rooms[1].positionY] = (uint8)ETileEnum::TE_Floor;//FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
				}
			}
		}
	}

	roomFound = false;
	//FRoom rooms[2];
	while (!(roomFound || zahl > 1000))
	{
		rooms[2].positionX = FMath::RandRange(1, gridSize - roomSize - 2);
		rooms[2].positionY = FMath::RandRange(1, gridSize - roomSize - 2);
		rooms[2].sizeX = roomSize;
		rooms[2].sizeY = roomSize;
		roomFound = !rooms[0].CollidesWithRoom(rooms[2]) && !rooms[1].CollidesWithRoom(rooms[2]);
		zahl++;

		if (roomFound)
		{
			for (int32 i = 0; i < roomSize; ++i)
			{
				for (int32 j = 0; j < roomSize; ++j)
				{
					matrix[i + rooms[2].positionX][j + rooms[2].positionY] = (uint8)ETileEnum::TE_Floor;//FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
				}
			}
		}
	}

	int dist1x = std::abs(rooms[0].GetCenterX() - rooms[1].GetCenterX());
	int dist1y = std::abs(rooms[0].GetCenterY() - rooms[1].GetCenterY());
	int dist2x = std::abs(rooms[0].GetCenterX() - rooms[2].GetCenterX());
	int dist2y = std::abs(rooms[0].GetCenterY() - rooms[2].GetCenterY());
	uint8 freeRoom;
	uint8 usedRoom;

	if (dist1x + dist1y < dist2x + dist2y)
	{
		freeRoom = 2;
		usedRoom = 1;
		if (rooms[0].GetCenterX() < rooms[1].GetCenterX())
		{
			for (int32 i = rooms[0].GetCenterX(); i < rooms[1].GetCenterX(); ++i)
			{
				matrix[i][rooms[0].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[0].GetCenterX(); i > rooms[1].GetCenterX(); --i)
			{
				matrix[i][rooms[0].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		if (rooms[0].GetCenterY() < rooms[1].GetCenterY())
		{
			for (int32 i = rooms[0].GetCenterY(); i < rooms[1].GetCenterY(); ++i)
			{
				matrix[rooms[1].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[0].GetCenterY(); i > rooms[1].GetCenterY(); --i)
			{
				matrix[rooms[1].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
	}
	else
	{
		freeRoom = 1;
		usedRoom = 2;
		if (rooms[0].GetCenterX() < rooms[2].GetCenterX())
		{
			for (int32 i = rooms[0].GetCenterX(); i < rooms[2].GetCenterX(); ++i)
			{
				matrix[i][rooms[0].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[0].GetCenterX(); i > rooms[2].GetCenterX(); --i)
			{
				matrix[i][rooms[0].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		if (rooms[0].GetCenterY() < rooms[2].GetCenterY())
		{
			for (int32 i = rooms[0].GetCenterY(); i < rooms[2].GetCenterY(); ++i)
			{
				matrix[rooms[2].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[0].GetCenterY(); i > rooms[2].GetCenterY(); --i)
			{
				matrix[rooms[2].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
	}

	dist1x = std::abs(rooms[freeRoom].GetCenterX() - rooms[0].GetCenterX());
	dist1y = std::abs(rooms[freeRoom].GetCenterY() - rooms[0].GetCenterY());
	dist2x = std::abs(rooms[freeRoom].GetCenterX() - rooms[usedRoom].GetCenterX());
	dist2y = std::abs(rooms[freeRoom].GetCenterY() - rooms[usedRoom].GetCenterY());

	if (dist1x + dist1y < dist2x + dist2y)
	{
		if (rooms[freeRoom].GetCenterX() < rooms[1].GetCenterX())
		{
			for (int32 i = rooms[freeRoom].GetCenterX(); i < rooms[1].GetCenterX(); ++i)
			{
				matrix[i][rooms[freeRoom].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[freeRoom].GetCenterX(); i > rooms[1].GetCenterX(); --i)
			{
				matrix[i][rooms[freeRoom].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		if (rooms[freeRoom].GetCenterY() < rooms[1].GetCenterY())
		{
			for (int32 i = rooms[freeRoom].GetCenterY(); i < rooms[1].GetCenterY(); ++i)
			{
				matrix[rooms[1].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[freeRoom].GetCenterY(); i > rooms[1].GetCenterY(); --i)
			{
				matrix[rooms[1].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
	}
	else
	{
		if (rooms[freeRoom].GetCenterX() < rooms[usedRoom].GetCenterX())
		{
			for (int32 i = rooms[freeRoom].GetCenterX(); i < rooms[usedRoom].GetCenterX(); ++i)
			{
				matrix[i][rooms[freeRoom].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[freeRoom].GetCenterX(); i > rooms[usedRoom].GetCenterX(); --i)
			{
				matrix[i][rooms[freeRoom].GetCenterY()] = (uint8)ETileEnum::TE_Floor;
			}
		}
		if (rooms[freeRoom].GetCenterY() < rooms[usedRoom].GetCenterY())
		{
			for (int32 i = rooms[freeRoom].GetCenterY(); i < rooms[usedRoom].GetCenterY(); ++i)
			{
				matrix[rooms[usedRoom].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
		else
		{
			for (int32 i = rooms[freeRoom].GetCenterY(); i > rooms[usedRoom].GetCenterY(); --i)
			{
				matrix[rooms[usedRoom].GetCenterX()][i] = (uint8)ETileEnum::TE_Floor;
			}
		}
	}

	//-----------------------------------------------------------------------------------

	for (int32 i = 0; i < gridSize; ++i)
	{
		for (int32 j = 0; j < gridSize; ++j)
		{
			uint8 rngType = matrix[i][j];// = FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
			uint8 rngTile = FMath::RandRange(0, tilesArray[rngType].tiles.Num() - 1);

			rngTile = FMath::RandRange(0, tilesArray[rngType].tiles.Num() - 1);
			if (tilesArray.IsValidIndex(rngType))
			{
				if (tilesArray[rngType].tiles.IsValidIndex(rngTile))
				{
					tilesArray[rngType].tiles[rngTile]->AddInstance(FTransform(FVector(i * tileSize, j * tileSize, 0)));
				}
			}
		}
	}


	AActor::ReregisterAllComponents();

	FVector NewLocation = FVector(0.f, 0.f, 0.f);
	FActorSpawnParameters SpawnInfo;
	APlayerController* pControll = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	pControll->GetPawn()->SetActorLocation(FVector(rooms[0].GetCenterX() * tileSize, rooms[0].GetCenterY() * tileSize, 142));
	GetWorld()->SpawnActor<APlayerStart>(NewLocation, FRotator::ZeroRotator, SpawnInfo);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("2"));
	
}

// Called every frame
void ADungeon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

