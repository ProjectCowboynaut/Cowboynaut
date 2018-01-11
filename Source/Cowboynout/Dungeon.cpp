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
	amountEnemies = 5;
}

// Called when the game starts or when spawned
void ADungeon::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	std::vector<std::vector<uint8>> matrix, tileMatrix;
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

	// setting void for every tile
	for (int32 i = 0; i < gridSize; ++i)
	{
		for (int32 j = 0; j < gridSize; ++j)
		{
			matrix[i][j] = (uint8)ETileEnum::TE_Void;//FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
		}
	}

	TArray<FRoom> rooms;

	FRoom r;
	FVector2D vec;
	vec = FVector2D(5, 5);
	int x = vec.X;
	int y = vec.Y;
	x = FMath::Clamp(x, 0, gridSize - 1);
	y = FMath::Clamp(y, 0, gridSize - 1);
	r = FRoom(FVector2D(x, y), FVector2D(roomSize, roomSize));
	rooms.Add(r);

	/**
	FRoom r2;
	vec = FVector2D(14, 13);
	x = vec.X;
	y = vec.Y;
	x = FMath::Clamp(x, 0, gridSize - 1);
	y = FMath::Clamp(y, 0, gridSize - 1);
	r2 = FRoom(FVector2D(x, y), FVector2D(roomSize, roomSize));
	rooms.Add(r2);

	if(r.isCollidingWith(r2))
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fak1")));
	/**/

	// getting rooms with random position in a circle

	for (int32 i = 0; i < 120; ++i)
	{
		FRoom r;
		bool colliding = false;
		int escape = 0;
		FVector2D vec;
		do
		{
			FVector2D vec;
			vec = getRandomPointInCircle(gridSize / 2);
			/*
			int x = vec.X + gridSize / 2;
			int y = vec.Y + gridSize / 2;
			*/
			int x = FMath::RandRange(1, gridSize);
			int y = FMath::RandRange(1, gridSize);
			x = FMath::Clamp(x, 0, gridSize - 1);
			y = FMath::Clamp(y, 0, gridSize - 1);
			r = FRoom(FVector2D(x, y), FVector2D(FMath::RandRange(2, roomSize), FMath::RandRange(2, roomSize)));
			for (int32 i = 0; i <= rooms.Max(); i++)
			{
				if (rooms.IsValidIndex(i))
				{
					if (rooms[i].isCollidingWith(r))
					{
						colliding = true;
						break;
					}
				}
			}
			escape++;
		} while (colliding && escape < 10000);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x: %d, y: %d"), x, y));
		if (colliding == false)
			rooms.Add(r);
	}

	// seperating rooms with Minkowski Difference
	/*
	for (int32 i = 0; i < rooms.Max(); i++)
	{
	int escape = 0;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fak1")));
	bool curIsColliding = true;
	while (curIsColliding && escape < 6300000)
	{
	escape++;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fak2")));
	for (int32 j = 0; j < rooms.Max(); j++)
	{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("fak3")));
	curIsColliding = false;
	if (rooms.IsValidIndex(i) && rooms.IsValidIndex(j))
	{
	if (j != i && rooms[i].isCollidingWith(rooms[j]))
	{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), escape));
	curIsColliding = true;
	rooms[i].location += rooms[i].getPenetrationVector(rooms[j]);
	break;
	}
	}
	}
	}
	}
	/**/

	// FOR TESTING stamping every room in matrix 
	for (int32 i = 0; i < rooms.Max(); i++)
	{
		if (rooms.IsValidIndex(i))
		{
			for (int32 ix = -rooms[i].extends.X; ix < rooms[i].extends.X + 1; ++ix)
			{
				for (int32 iy = -rooms[i].extends.Y; iy < rooms[i].extends.Y + 1; ++iy)
				{
					int x = rooms[i].location.X + ix;
					int y = rooms[i].location.Y + iy;
					x = FMath::Clamp(x, 0, gridSize - 1);
					y = FMath::Clamp(y, 0, gridSize - 1);
					matrix[x][y] = (uint8)ETileEnum::TE_Floor;
				}
			}
		}
	}

	TArray<FRoom*> unconnectedRooms;
	for (int32 i = 0; i < rooms.Max(); i++)
	{
		if (rooms.IsValidIndex(i))
		{
			unconnectedRooms.Add(&rooms[i]);
			//unconnectedRooms[i] = &rooms[i];
		}
	}

	FVector2D curDistVec;
	while (unconnectedRooms.Num() > 1 /*unconnectedRooms.IsValidIndex(0)*/)
	{
		int32 xyz = unconnectedRooms.Num();
		int32 minDistIndex;
		FVector2D minDistVec;
		for (int32 j = 1; j < unconnectedRooms.Max(); j++)
		{
			if (j == 1)
			{
				minDistVec = unconnectedRooms[0]->location - unconnectedRooms[j]->location;
				minDistIndex = j;
			}
			if (unconnectedRooms.IsValidIndex(j))
			{
				curDistVec = unconnectedRooms[0]->location - unconnectedRooms[j]->location;
				if (curDistVec.DistSquared < minDistVec.DistSquared)
				{
					minDistVec = unconnectedRooms[0]->location - unconnectedRooms[j]->location;
					minDistIndex = j;
				}
			}
		}

		// draw connection between rooms

		int i = minDistVec.X;
		int abs = FMath::Abs(i);
		int sign = 0;
		if (abs != 0)
			sign = i / abs;
		int esc = 0;
		while (i != 0 /*&& esc < 1000*/)
		{
			int x = unconnectedRooms[0]->location.X + i * sign;
			int y = unconnectedRooms[0]->location.Y;
			int y2 = y + 1;
			x = FMath::Clamp(x, 0, gridSize - 1);
			y = FMath::Clamp(y, 0, gridSize - 1);
			y2 = FMath::Clamp(y2, 0, gridSize - 1);
			matrix[x][y] = (uint8)ETileEnum::TE_Floor;
			matrix[x][y2] = (uint8)ETileEnum::TE_Floor;
			if (i < 0)
				i++;
			else if (i > 0)
				i--;
			esc++;
			//i -= sign;
		}

		i = minDistVec.Y;
		abs = FMath::Abs(i);
		sign = 0;
		if (abs != 0)
			sign = i / abs;
		esc = 0;
		while (i != 0 /*&& esc < 1000*/)
		{
			int x = unconnectedRooms[0]->location.X;
			int x2 = x + 1;
			int y = unconnectedRooms[0]->location.Y + i * sign;
			x = FMath::Clamp(x, 0, gridSize - 1);
			y = FMath::Clamp(y, 0, gridSize - 1);
			x2 = FMath::Clamp(x2, 0, gridSize - 1);
			matrix[x][y] = (uint8)ETileEnum::TE_Floor;
			matrix[x2][y] = (uint8)ETileEnum::TE_Floor;
			if (i < 0)
				i++;
			else if (i > 0)
				i--;
			esc++;
			//i -= sign;
		}
		/*
		*/

		unconnectedRooms[0] = unconnectedRooms[minDistIndex];
		unconnectedRooms.RemoveAt(minDistIndex);


	}
	/*
	*/



	tileMatrix = matrix;

	for (int32 i = 0; i < gridSize; ++i)
	{
		for (int32 j = 0; j < gridSize; ++j)
		{
			if (matrix[i][j] == (uint8)ETileEnum::TE_Floor)
			{
				// LL
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Floor) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Floor))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallLL;
				}
				// UU
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Void) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Floor))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallUU;
				}
				// LL
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Floor) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Floor))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallRR;
				}
				// DD
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Floor) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Void))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallDD;
				}







				// UL
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Void) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Floor))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallUL;
				}
				// UR
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Void) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Floor))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallUR;
				}
				// DR
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Floor) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Void))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallDR;
				}
				// DL
				if ((i - 1 < 0 || matrix[i - 1][j] == (uint8)ETileEnum::TE_Void) &&		//check if left tile is void
					(j - 1 < 0 || matrix[i][j - 1] == (uint8)ETileEnum::TE_Floor) &&		//check if up tile is ground
					(i + 1 >= gridSize || matrix[i + 1][j] == (uint8)ETileEnum::TE_Floor) &&		//check if right tile is ground
					(j + 1 >= gridSize || matrix[i][j + 1] == (uint8)ETileEnum::TE_Void))			//check if down tile is ground
				{
					tileMatrix[i][j] = (uint8)ETileEnum::TE_WallDL;
				}
			}
		}
	}



	// render matrix with ISCM
	for (int32 i = 0; i < gridSize; ++i)
	{
		for (int32 j = 0; j < gridSize; ++j)
		{
			uint8 rngType = tileMatrix[i][j];  //**/ FMath::RandRange(0, ((uint8)ETileEnum::TE_Max) - 1);
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

	FRotator spwnRot = FRotator();
	for (int32 i = 0; i < amountEnemies; ++i)
	{
		FVector v = FVector();
		int rndRoom;
		do
		{
			rndRoom = FMath::RandRange(1, rooms.Max() - 1);

		} while (!rooms.IsValidIndex(rndRoom));

		//AActor* a = GetWorld()->SpawnActor<AActor>(enemyBP, v, r, ActorSpawnParams);//[projectile];
		FVector rndVec = FVector((float)FMath::RandRange(-100, 100) / 100.f, (float)FMath::RandRange(-100, 100) / 100.f, 0);
		UWorld* w = GetWorld();
		AEnemy* a = w->SpawnActor<AEnemy>(enemyBP, v, spwnRot, ActorSpawnParams);
		if (a)
			a->SetActorLocation(FVector(rooms[rndRoom].location.X * tileSize + rndVec.X, rooms[rndRoom].location.Y * tileSize + rndVec.Y, 150));
	}

	/*int exitRoom = rooms.Max();
	while (exitRoom > 0)
	{
		if (rooms.IsValidIndex(rooms.Max() - exitRoom))
		{
			GetWorld()->SpawnActor<AActor>(TPExit, FVector(rooms[rooms.Max() - exitRoom].location.X * tileSize, rooms[rooms.Max() - exitRoom].location.Y * tileSize, 142), FRotator(), ActorSpawnParams);
			exitRoom = 0;
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red, FString::Printf(TEXT("fick")));
		exitRoom--;
	}*/

	//moving player
	/*
	APlayerController* pControll = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pControll->GetPawn())
		pControll->GetPawn()->SetActorLocation(FVector(rooms[0].location.X * tileSize, rooms[0].location.Y * tileSize, 142));
	*/

}

// Called every frame
void ADungeon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector2D ADungeon::getRandomPointInCircle(float radius)
{
	float t = 0.000002 * FMath::RandRange(1, 1000000)  * 3.14159265359;
	float u = FMath::RandRange(1, 1000000) * 0.000001;
	u = FMath::RandRange(1, 1000000) *0.000001;
	float r = .0f;
	if (u > 1) r = 2 - u;
	else r = u;
	//GEngine->AddOnScreenDebugMessage(-1, 25.f, FColor::Red, FString::Printf(TEXT("t: %f, u: %f, r: %f"), t, u, r));
	float x = radius * r*FMath::Cos(t);
	float y = radius * r*FMath::Sin(t);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x: %f, y: %f"), x, y));
	return FVector2D(x, y);
}

