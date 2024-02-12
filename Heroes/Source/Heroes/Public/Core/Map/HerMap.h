// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "HerMap.generated.h"

UCLASS()
class HEROES_API AHerMap : public AActor
{
	GENERATED_BODY()

	// using uint32 = unsigned int;

public:	
	// Устанавливает значения по умолчанию
	AHerMap();

protected:
	// Вызывается при запуске игры или при появлении
	virtual void BeginPlay() override;

public:	
	// Вызывается каждый кадр
	virtual void Tick(float DeltaTime) override;

	/** Please add a variable description*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<USceneComponent> DefaultSceneRoot;
	// Коммпонент процедурной сетки ландшафта
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
		TObjectPtr<UProceduralMeshComponent> TerrainMesh;
	// Количество вершин по координате x
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int X_VertexCount;
	// Количество вершин по координате y
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int Y_VertexCount;
	// Размер одной секции ландшафта
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		double CellSize;
	// Количество вершин в сетке ландшафта
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FVector> Vertices;
	// Количество треугольников в сетке ландшафта
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<int> Triangles;
	// Нормали
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FVector> Normals;
	// UV карта
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FVector2D> UV0;
	/* Цветовые вершины
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FLinearColor> VertexColors;*/
	// 
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FProcMeshTangent> Tangents;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		float Frequency = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		float Amplitude = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		/*uint8*/int Octave = 1;
	

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Создает ландшафт
		/// </summary>
		/// <param name="X_Veretex"></param> Количество вершин по координате X
		/// <param name="Y_Vertex"></param> Количество вершин по координате Y
		/// <param name="Size"></param> Размер всей секции
		void GenerateTerrain(int X_Veretex = 10, int Y_Vertex = 10, double Size = 1000);

	UFUNCTION(BlueprintCallable)
		// Уничтожает ландшафт
		void DestroyTerrain();

	UFUNCTION(BlueprintCallable)
		// Очищает ландшафт
		void ResetTerrain();

	//UFUNCTION(BlueprintCallable)
		// Рассчет нормалей вершин
		void Normal();
};
