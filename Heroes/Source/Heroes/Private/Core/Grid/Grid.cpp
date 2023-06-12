// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Grid/Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "..\..\..\Public\Core\Grid\Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedMesh");

}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::SpawnGrid(FVector CenterLocation, FVector TileSize, FIntVector TileCount, bool UseEnviroment)
{
	GridCenterLocation = CenterLocation;
	GridTileSize = TileSize;
	GridTileCount = TileCount;
	//GridBottomLeftCenterLocation = GridCenterLocation - (GridTileCount / 2) * GridTileSize;
}

FVector AGrid::SnapVectorToVector(FVector V1, FVector V2)
{
	return FVector(FMath::GridSnap(V1.Component(0), V2.Component(0)), FMath::GridSnap(V1.Component(1), V2.Component(1)), FMath::GridSnap(V1.Component(2), V2.Component(2)));
}

FVector AGrid::CalculateCenter()
{
	FVector V = GridTileSize * FVector(1.5f, 1.0f, 1.0f);
	FVector Center = SnapVectorToVector(GridCenterLocation, V);

	return Center;
}

FVector AGrid::CalculateBottom()
{
	FVector V = GridTileSize * FVector(1.5f, 1.0f, 1.0f);
	FVector Center = SnapVectorToVector(GridCenterLocation, V);
	FVector V2 = (GridTileCount / FIntVector(3, 2, 1)) * GridTileSize;

	return Center - SnapVectorToVector(V2, V);
}

