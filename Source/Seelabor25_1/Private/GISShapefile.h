// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealGDAL.h"
#include "GDALHelpers.h"

#include "CoreMinimal.h"
#include "GISDataFile.h"
#include "GISShapefile.generated.h"

/**
 * 
 */
UCLASS()
class UGISShapefile : public UGISDataFile
{
	GENERATED_BODY()

public: 
	UGISShapefile();
	~UGISShapefile();

	void Initialize(FString path, float minX, float maxX, float minY, float maxY, float sizeX, float sizeY);
	TArray<FString> GetAttributes(FString AttributeName) override;

private: 
	void CalculateAttributes(OGRLayer* poLayer);
	TArray<OGRLayer*> GetLayers(GDALDatasetRef poDS);

	TArray<FVector> CalCoordinates(OGRLayer* poLayer);
	TArray<FVector> CalCooFromPointGeom(OGRLayer* poLayer);
	TArray<FVector> CalCooFromLineGeom(OGRLayer* poLayer);
	TArray<FVector> CalCooFromPolygonGeom(OGRLayer* poLayer);
	TArray<FVector> CalCooFromMultiPolygonGeom(OGRLayer* poLayer);

	void ReprojectCoordinates(OGRSpatialReference oSourceSRS, TArray<FVector>* pointRef);
	void ReprojectCoordinates(OGRSpatialReference oSourceSRS, OGRPoint* pointRef);
	void ReprojectCoordinates(OGRLayer* poLayer, std::vector<FVector>* cooRef);

	OGRLayer* ReprojectLayer(OGRLayer* inLayer);
	FVector ConvertToVector(OGRPoint po);
	TArray<FVector> MakePointsInPolygon(std::vector<FVector> polygon);
	bool pnpoly(std::vector<FVector> vert, FVector p);
	TArray<FVector> MakePointsInLine(OGRPoint start, OGRPoint end);

private: 
	int LayerCount;
	char** fileList;
	GDALDatasetRef data;
	TArray<OGRLayer*> layers;

protected:
	TArray<FString> AttributeNames;

	template<typename T>
	TArray<T> ConvertToUnrealArray(std::vector<T> v);

};
