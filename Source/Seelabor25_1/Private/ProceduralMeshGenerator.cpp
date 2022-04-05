// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMeshGenerator.h"
#include "DataManager.h"
#include "GISRasterfile.h"
#include <algorithm>


#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <ProceduralMeshComponent/Public/KismetProceduralMeshLibrary.h>

// Sets default values
AProceduralMeshGenerator::AProceduralMeshGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	RestrictedArea = CreateDefaultSubobject<UProceduralMeshComponent>("Sperrgebiete");

	SetRootComponent(ProceduralMesh);

	ProceduralMesh->bUseAsyncCooking = true;
	ProceduralMesh->bEnableAutoLODGeneration = true;
	ProceduralMesh->bCastCinematicShadow = false; 

	RestrictedArea->bUseAsyncCooking = true;
	RestrictedArea->bEnableAutoLODGeneration = true;
	RestrictedArea->bCastCinematicShadow = false;

	section = 0; 
}

// Called when the game starts or when spawned
void AProceduralMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProceduralMeshGenerator::AddSection(TArray<int32> height, int XLength, int YLength, float GridSize) 
{
	TArray<FVector> Vertices; 
	TArray<FVector2D> UVLocations; 
	TArray<FLinearColor> VertexColors;


	int splitIdx = 10; 


	Vertices.Empty();
	UVLocations.Empty();
	VertexColors.Empty();

	for (int x = 0; x < XLength; x++)
	{
		for (int y = 0; y < YLength; y++)
		{
			Vertices.Add(FVector(x * GridSize, y * GridSize, height[y * XLength + x]));
			VertexColors.Add(FLinearColor(0, 150, height[y * XLength + x]));
			UVLocations.Add(FVector2D(x / XLength, y / YLength));
		}
	}

	TArray<int32> Triangles;
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(XLength, YLength, false, Triangles);
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	// UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVLocations, Normals, Tangents); 

	// Draw Mesh
	// ProceduralMesh->ClearAllMeshSections();
	ProceduralMesh->CreateMeshSection_LinearColor(section, Vertices, Triangles, Normals, UVLocations, VertexColors, Tangents, false);
	section++;
}

void AProceduralMeshGenerator::GetPixelFromPNG(FString path, TArray<int32>& height, FVector2D& size) 
{
	UGISRasterfile* file = NewObject<UGISRasterfile>();
	// file->GetHeight("C://Users//IGD//1-Seelabor//02-Visualisierung//PudgetSound_001.png");
	// file->GetHeight("C://Users//IGD//1-Seelabor//02-Visualisierung//PudgetSound_TopRight.png");
	height = file->GetHeight(path, size); 

}

FVector2D AProceduralMeshGenerator::GetPixelFromUTexture(UTexture2D* myTexture2D, TArray<int32>& height)
{
	FVector2D size;

	TextureCompressionSettings OldCompressionSettings = myTexture2D->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = myTexture2D->MipGenSettings; 
	bool OldSRGB = myTexture2D->SRGB;

	myTexture2D->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	myTexture2D->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	myTexture2D->SRGB = false;
	myTexture2D->UpdateResource();

	const FColor* FormatedImageData = static_cast<const FColor*>(myTexture2D->PlatformData->Mips[0].BulkData.LockReadOnly());

	size = FVector2D(myTexture2D->GetSizeX(), myTexture2D->GetSizeY());

	for (int32 X = 0; X < size.X; X++)
		for (int32 Y = 0; Y < size.Y; Y++)
			height.Add(FormatedImageData[Y * (int32)size.X + X].R);


	myTexture2D->PlatformData->Mips[0].BulkData.Unlock();

	return size;
}

////////////////////////////////////////////////////////////////////////////////////////
// Bathymetrie
///////////////////////////////////////////////////////////////////////////////////////
void AProceduralMeshGenerator::GenerateBathymetrieData() 
{
	// Get Data
	UCloudData* d = NewObject<UCloudData>();
	d->InitializeXYZFile("C://Users//IGD//1-Seelabor//02-Visualisierung//Seelabor25_1//Source//load//Bathymetry.txt", 
		"Bathymetry", 
		FColor::Blue);
	TArray<FVector2D> locations = d->Locations2D; 
	TArray<float> depth = d->depth; 

	// Add Mesh
	TArray<FVector> Vertices;
	TArray<FVector2D> UVLocations;
	TArray<FColor> VertexColors;

	// clear 
	Vertices.Empty(); 
	UVLocations.Empty(); 
	VertexColors.Empty(); 
	ProceduralMesh->ClearAllMeshSections(); 

	// size
	int sizeX = 62; 
	int sizeY = locations.Num() / sizeX; 
	int gridSize = 50; 
	float min = FMath::Min(depth);

	// add vertices
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			int i = y * sizeX + x; 
			Vertices.Add(FVector(locations[i].X, locations[i].Y, depth[i]));
			VertexColors.Add(MapDepthToColor(depth[i], min));
			UVLocations.Add(FVector2D(x / sizeX, y / sizeY));
		}
	}

	TArray<int32> Triangles;
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(sizeX, sizeY, false, Triangles);
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	// UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVLocations, Normals, Tangents); 

	// Draw Mesh
	// ProceduralMesh->ClearAllMeshSections();
	ProceduralMesh->CreateMeshSection(section, Vertices, Triangles, Normals, UVLocations, VertexColors, Tangents, false);
	section++;

}

FColor AProceduralMeshGenerator::MapDepthToColor(float depth, float range) 
{
	FColor color; 
	FColor lightBlue = FColor(173, 216, 230, 255);
	FColor darkBlue  = FColor(65, 105, 225, 255); 
	FColor darkLand = FColor(75, 118,59, 255); 
	FColor lightLand = FColor(72, 135, 50, 255);
	float PixelsWidthAttemptingToFillIn; 

	float mRed, mGreen, mBlue, nRed, nGreen, nBlue; 

	// water
	if (depth < 0) {
		PixelsWidthAttemptingToFillIn = range;
		mRed = (darkBlue.R - lightBlue.R) / PixelsWidthAttemptingToFillIn;
		mGreen = (darkBlue.G - lightBlue.G) / PixelsWidthAttemptingToFillIn;
		mBlue = (darkBlue.B - lightBlue.B) / PixelsWidthAttemptingToFillIn;

		nRed = mRed * depth + lightBlue.R;
		nGreen = mGreen * depth + lightBlue.G;
		nBlue = mBlue * depth + lightBlue.B;

		color = FColor(nRed, nGreen, nBlue);
	}
	// land 
	else {

		PixelsWidthAttemptingToFillIn = 1.0f;
		depth = FMath::RandRange(0.0f, 1.0f); 
		mRed = (darkLand.R - lightLand.R) / PixelsWidthAttemptingToFillIn;
		mGreen = (darkLand.G - lightLand.G) / PixelsWidthAttemptingToFillIn;
		mBlue = (darkLand.B - lightLand.B) / PixelsWidthAttemptingToFillIn;

		nRed = mRed * depth + lightLand.R;
		nGreen = mGreen * depth + lightLand.G;
		nBlue = mBlue * depth + lightLand.B;

		color = FColor(nRed, nGreen, nBlue);
	}

	

	return color; 
}

float AProceduralMeshGenerator::smoothstep(float edge0, float edge1, float x) 
{
	
	x = FMath::Clamp(x, edge0, edge1); 



	
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}

///////////////////////////////////////////////////////////////////////////////////////
// Lines
//////////////////////////////////////////////////////////////////////////////////////

UProceduralMeshComponent* AProceduralMeshGenerator::AddAreaMesh(TArray<FVector2D> Outline)
{
	float minX, maxX, minY, maxY; 
	minX = Outline[0].X; 
	maxX = Outline[0].X; 
	minY = Outline[0].Y; 
	maxY = Outline[0].Y; 

	for (FVector2D v : Outline) {
		if (v.X < minX) minX = v.X;
		else if (v.X > maxX) maxX = v.X; 

		if (v.Y < minY) minY = v.Y;
		else if (v.Y > maxY) maxY = v.Y; 
	}

	// Add Mesh
	TArray<FVector> Vertices;
	TArray<FVector2D> UVLocations;
	TArray<FColor> VertexColors;

	// clear 
	Vertices.Empty();
	UVLocations.Empty();
	VertexColors.Empty();

	

	// add vertices
	float gridSize = 1; 
	float sizeX = (maxX - minX) / gridSize; 
	float sizeY = (maxY - minY) / gridSize;

	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			FVector v = FVector(minX + gridSize * x, minY + gridSize * y, 10.0f); 
			Vertices.Add(v);
			VertexColors.Add(IsInside(v, Outline));
			UVLocations.Add(FVector2D(x / sizeX, y / sizeY));
		}
	}

	TArray<int32> Triangles;
	UKismetProceduralMeshLibrary::CreateGridMeshTriangles(sizeX, sizeY, false, Triangles);
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	// UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVLocations, Normals, Tangents); 

	// Draw Mesh
	// ProceduralMesh->ClearAllMeshSections();
	RestrictedArea->CreateMeshSection(0, Vertices, Triangles, Normals, UVLocations, VertexColors, Tangents, false);

	return RestrictedArea; 
}

FColor AProceduralMeshGenerator::IsInside(FVector point, TArray<FVector2D>& points) 
{
	int i, j, nvert = points.Num();
	bool c = false;

	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((points[i].Y > point.Y) != (points[j].Y) > point.Y) && (point.X < (points[j].X - points[i].X) * (point.Y - points[i].Y) / (points[j].Y - points[i].Y) + points[i].X))
			c = !c;
	}

	if (c) {
		return FColor::Red; 
	}
	else {
		return FColor(1, 1, 1, 0); 
	}
}

