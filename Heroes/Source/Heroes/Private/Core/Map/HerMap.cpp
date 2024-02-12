// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Map/HerMap.h"
#include "KismetProceduralMeshLibrary.h"

// Устанавливает значения по умолчанию
AHerMap::AHerMap()
{
 	// Настройте этого актера на вызов Tick() в каждом кадре. Вы можете отключить эту функцию, чтобы повысить производительность, если она вам не нужна.
	PrimaryActorTick.bCanEverTick = false;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	RootComponent = DefaultSceneRoot;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TerrainMesh");
	TerrainMesh->SetupAttachment(DefaultSceneRoot);

}

// Вызывается при запуске игры или при появлении
void AHerMap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Вызывается каждый кадр
void AHerMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 
void AHerMap::GenerateTerrain(int X_Veretex, int Y_Vertex, double Size)
{
	X_VertexCount = X_Veretex;
	Y_VertexCount = Y_Vertex;
	CellSize = Size;

	// Очищаем предыдущую сетку, если она есть
	ResetTerrain();

	double X = (CellSize / X_VertexCount) * ((X_VertexCount-1) / 2.0f) * -1;
	double Y = ((Y_VertexCount-1) / 2.0f) * (CellSize / Y_VertexCount) * -1;
	FVector VertexCoord = FVector(X, Y, 0);

	for (int iY = 0; iY < Y_VertexCount; iY++) {
		for (int iX = 0; iX < X_VertexCount; iX++) {
			// Добавляем в массив вершины сетки
			double vX = CellSize / X_VertexCount * iX;
			double vY = CellSize / Y_VertexCount * iY;
			float vZ = 0;

			// Шум перлина
			if (Octave > 1) {
				for (int j = 0; j < Octave; j++) {
					vZ += FMath::PerlinNoise2D(FVector2D((VertexCoord.X + vX) * Frequency, (VertexCoord.Y + vY) * Frequency)) * Amplitude;

					Frequency *= 2;
					Amplitude /= 2;
				}
			} else vZ = FMath::PerlinNoise2D(FVector2D((VertexCoord.X + vX) * Frequency, (VertexCoord.Y + vY) * Frequency)) * Amplitude;
			
			Vertices.Add(VertexCoord + FVector(vX, vY, vZ));
			UV0.Add(FVector2D(iX, iY));

			uint32 Factor = 0;
			if ((X_VertexCount - 1) > iX && (Y_VertexCount - 1) > iY) {
				if (X_VertexCount >= Y_VertexCount) {
					Factor = X_VertexCount * iY + iX;
				}
				else Factor = Y_VertexCount* iY + iX;

				// Левый нижний треугольник квадрата
				Triangles.Add(Factor);
				Triangles.Add(Factor + X_VertexCount);
				Triangles.Add(Factor + 1);

				// Правый верхний треугольник квадрата
				Triangles.Add(Factor + X_VertexCount);
				Triangles.Add(Factor + X_VertexCount + 1);
				Triangles.Add(Factor + 1);
			}

			// После того как мы заполним первый ряд вершин у нас появится первый треугольник и мы сможем начать считать нормали для ввершин
			if (iY >= 1) {

			}
		}
	}

	// Рассчет нормалей и тангенсов путем стандартного функционала движка
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);

																						// VertexColors
	TerrainMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, TArray<FLinearColor>(), Tangents, false);
}

void AHerMap::DestroyTerrain()
{
	Vertices.Empty();
	Triangles.Empty();
	UV0.Empty();
}

void AHerMap::ResetTerrain()
{
	Vertices.Reset();
	Triangles.Reset();
	UV0.Reset();
}

void AHerMap::Normal()
{
	// Простейший подход заключается в том, 
	// чтобы вычислить нормаль грани для 
	// треугольника и использовать ее в 
	// качестве нормали для всех трех вершин.

	// Сперва вычислим два вектора, лежащих в полскости треугольника

}
