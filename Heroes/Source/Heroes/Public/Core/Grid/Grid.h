// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

class UInstancedStaticMeshComponent;


UCLASS()
class HEROES_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	UInstancedStaticMeshComponent* InstancedMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	FVector GridCenterLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	FVector GridTileSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	FIntVector GridTileCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mesh")
	FVector GridBottomLeftCenterLocation;

	//UFUNCTION(BlueprintCallable)
		void SpawnGrid(FVector CenterLocation, FVector TileSize, FIntVector TileCount, bool UseEnviroment);

		inline FVector SnapVectorToVector(FVector V1, FVector V2);

		FVector CalculateCenter();

		FVector CalculateBottom();

};
