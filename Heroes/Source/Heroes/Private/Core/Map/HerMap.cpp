// Fill out your copyright notice in the Description page of Proiect Settings.


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
void AHerMap::GenerateTerrain(int Res, double Size)
{
	Resolution = Res;
	CellSize = Size;

	// Количество вершин в сетке ландшафта
	TArray<FVector> Vertices;
	// Количество треугольников в сетке ландшафта
	TArray<int> Triangles;
	// Нормали
	TArray<FVector> Normals;
	// UV карта
	TArray<FVector2D> UV0;

	// Очищаем предыдущую сетку, если она есть
	//ResetTerrain();

	double offset = (CellSize / Resolution) * ((Resolution - 1) / 2.0f) * -1;
	FVector VertexCoord = FVector(offset, offset, 0);

	for (int y = 0; y < Resolution; y++) {
		for (int x = 0; x < Resolution; x++) {
			// Добавляем в массив вершины сетки
			double vX = CellSize / Resolution * x;
			double vY = CellSize / Resolution * y;
			float vZ = 0;

			// Шум перлина
			if (Octave > 1) {
				for (int i = 0; i < Octave; i++) {
					
					float tFreq = Frequency;
					float tAmp = Amplitude;
					//vZ += PerlinNoise((VertexCoord.X + vX) /*/ CellSize*/ * tFreq, (VertexCoord.Y + vY) /*/ CellSize*/ * tFreq) * tAmp;
					vZ += FMath::PerlinNoise2D(FVector2D((VertexCoord.X + vX) / CellSize * tFreq, (VertexCoord.Y + vY) / CellSize * tFreq)) * tAmp;

					tFreq *= 2;
					tAmp /= 2;
				}
			} else vZ = FMath::PerlinNoise2D(FVector2D((VertexCoord.X + vX) * Frequency, (VertexCoord.Y + vY) * Frequency)) * Amplitude;
			
			Vertices.Add(VertexCoord + FVector(vX, vY, vZ));
			UV0.Add(FVector2D(x, y) * UV_Scale * 0.02);

			uint32 Factor = 0;
			if ((Resolution - 1) > x && (Resolution - 1) > y) {
				Factor = Resolution * y + x;

				// Левый нижний треугольник квадрата
				Triangles.Add(Factor);
				Triangles.Add(Factor + Resolution);
				Triangles.Add(Factor + 1);

				// Правый верхний треугольник квадрата
				Triangles.Add(Factor + Resolution);
				Triangles.Add(Factor + Resolution + 1);
				Triangles.Add(Factor + 1);
			}
			

			// После того как мы заполним первый ряд вершин у нас появится первый треугольник и мы сможем начать считать нормали для ввершин
			if (y >= 1) {

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
	//Vertices.Empty();
	//Triangles.Empty();
	//UV0.Empty();
}

void AHerMap::ResetTerrain()
{
	//Vertices.Reset();
	//Triangles.Reset();
	//UV0.Reset();
}

void AHerMap::Normal()
{
	// Простейший подход заключается в том, 
	// чтобы вычислить нормаль грани для 
	// треугольника и использовать ее в 
	// качестве нормали для всех трех вершин.

	// Сперва вычислим два вектора, лежащих в полскости треугольника

}

float AHerMap::PerlinNoise(float x, float y)
{
	// Определить угловые координаты ячейки сетки
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	// Вычисление весовых коэфициентов интерполяции
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Вычисляем и интерполируем два верхних угла
	float n0 = DotGridGradient(x0, y0, x, y);
	float n1 = DotGridGradient(x1, y0, x, y);
	float ix0 = CubicInterpolate(n0, n1, sx);

	// Вычислить и интерполировать два нижних угла
	n0 = DotGridGradient(x0, y1, x, y);
	n1 = DotGridGradient(x1, y1, x, y);
	float ix1 = CubicInterpolate(n0, n1, sx);

	// Последний шаг: интерполировать между двумя ранее интерполированными значениями, теперь в y
	float value = CubicInterpolate(ix0, ix1, sy);

	return value;
}

float AHerMap::DotGridGradient(int ix, int iy, float x, float y)
{
	FVector2D Gradient = RandomGradient(ix, iy);

	// Вычислить вектор расстояния
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Вычислить скалярное произведение
	return (dx * Gradient.X + dy * Gradient.Y);

	return 0.0f;
}

FVector2D AHerMap::RandomGradient(int x, int y)
{
	// Отсутствие заранее рассчитанных градиентов означает, что это работает для любого количества координат сетки
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = x, b = y;
	a *= 3284157443;

	b ^= a << s | a >> (w - s);
	b *= 1911520717;

	a ^= b << s | b >> (w - s);
	a *= 2048419325;
	float random = a * (PI / ~(~0u >> 1)); // в [0, 2*Pi]

	// Создаём вектор из угла
	FVector2D v(FMath::Sin(random), FMath::Cos(random));


	return v;
}

float AHerMap::CubicInterpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}
