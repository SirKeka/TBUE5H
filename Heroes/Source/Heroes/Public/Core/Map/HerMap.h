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
	// Количество вершин по одной гране меша
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int Resolution = 10;
	// Размер одной секции ландшафта
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		double CellSize = 1000.0;
	/* Цветовые вершины
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FLinearColor> VertexColors;*/
	// 
	UPROPERTY(BlueprintReadWrite, Category = "Default")
		TArray<FProcMeshTangent> Tangents;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
		int UV_Scale = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		float Frequency = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		float Amplitude = 1.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerlinNoise")
		/*uint8*/int Octave = 12;
	

	UFUNCTION(BlueprintCallable)
		/// <summary>
		/// Создает ландшафт
		/// </summary>
		/// <param name="Res"></param> Количество вершин по одной гране меша
		/// <param name="Size"></param> Размер всей секции
		void GenerateTerrain(int Res, double Size);

	UFUNCTION(BlueprintCallable)
		// Уничтожает ландшафт
		void DestroyTerrain();

	UFUNCTION(BlueprintCallable)
		// Очищает ландшафт
		void ResetTerrain();

	//UFUNCTION(BlueprintCallable)
		// Рассчет нормалей вершин
		void Normal();
	// Простой шум Перлина в координатах x, y
	float PerlinNoise(float x, float y);

private:
	// Вычисление скалярного произведения 
	// векторов расстояния и случайного
	// градиента в указанной точке.
	float DotGridGradient(int ix, int iy, float x, float y);

	FVector2D RandomGradient(int x, int y);

	inline float CubicInterpolate(float a0, float a1, float w);
};
