// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealGDAL.h"
#include "GDALHelpers.h"

#include "CoreMinimal.h"

/**
 * Contains Data of Shapefile
 */
class Shapefile
{
public:
	Shapefile();
	Shapefile(const FString path);

	void CalculateAttributes(OGRLayer* poLayer);

	~Shapefile();

private:
	int LayerCount;
	char** fileList;
	GDALDatasetRef data;
	std::vector<OGRLayer*> layers;
	std::vector<FVector> coordinates; 
	std::string Type; 
	std::vector<FString> attributes; 

	std::vector<FVector> CalCooFromPointGeom(OGRLayer* poLayer);
	std::vector<FVector> CalCooFromLineGeom(OGRLayer* poLayer);
	std::vector<FVector> CalCooFromMultiPolygonGeom(OGRLayer* poLayer);

	std::vector<FVector> CalCooFromPolygonGeom(OGRLayer* poLayer);
	FVector ConvertToVector(OGRPoint po);

	std::vector<FVector> MakePointsInPolygon(std::vector<FVector> polygon);
	bool pnpoly(std::vector<FVector> vert, FVector p);

	std::vector<FVector> MakePointsInLine(OGRPoint start, OGRPoint end);

	void ReprojectCoordinates(OGRSpatialReference oSourceSRS, std::vector<FVector>* pointRef);
	void ReprojectCoordinates(OGRSpatialReference oSourceSRS, OGRPoint* pointRef);
	void ReprojectCoordinates(OGRLayer* poLayer, std::vector<FVector>* cooRef);
	OGRLayer* ReprojectLayer(OGRLayer* poLayer);

	std::vector<OGRLayer*> GetLayers(GDALDatasetRef poDS);
	std::vector<FVector> CalCoordinates(OGRLayer* layer);

	TArray<FVector> ConvertToUnrealArray(std::vector<FVector> v); 

public: 
	TArray<FVector> GetCoordinates() { return (ConvertToUnrealArray(coordinates)); };
	TArray<FString> GetAttributes(); 
	TArray<FVector2D> GetImageCoordinates(float sizeX, float sizeY);
	std::string GetType(); 

	template <typename T>
	TArray<T> ConvertToUnrealArray(std::vector<T> v); 
};



