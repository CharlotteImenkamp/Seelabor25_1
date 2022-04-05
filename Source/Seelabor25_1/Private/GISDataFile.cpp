// Fill out your copyright notice in the Description page of Project Settings.


#include "GISDataFile.h"

TArray<FVector> UGISDataFile::CalculateCoordinates(FString path)
{
	return TArray<FVector>();
}

TArray<FString> UGISDataFile::GetAttributes(FString AttributeName)
{
	return TArray<FString>();
}

TArray<FVector2D> UGISDataFile::GetImageCoordinates()
{
	TArray<FVector2D> imageCoordinates;

	std::vector<float> xCoordinates, yCoordinates;

	for (FVector p : coordinates) {
		xCoordinates.push_back(p.X);
		yCoordinates.push_back(p.Y);
	}

	FVector worldCoordinate;
	for (size_t i = 0; i < coordinates.Num(); i++)
	{
		worldCoordinate = GetWorldCoordinate(xCoordinates[i], yCoordinates[i], SizeX, SizeY, minmaxX.first, minmaxX.second, minmaxY.first, minmaxY.second);
		imageCoordinates.Emplace(FVector2D(worldCoordinate.X, worldCoordinate.Y));
	}

	return imageCoordinates;
}

FVector UGISDataFile::GetWorldCoordinate(float x, float y, float SizeX, float SizeY, float minX, float maxX, float minY, float maxY) {
	float xWorld = (x - minX) / (maxX - minX) * SizeX;
	float yWorld = (y - minY) / (maxY - minY) * SizeY;

	return FVector(xWorld, yWorld, 0.0);
}

FVector UGISDataFile::GetEPSG(float x, float y, float SizeX, float SizeY, float minX, float maxX, float minY, float maxY) {
	float xReprojected = (x / SizeX) * (maxX - minX) + minX;
	float yReprojected = (y / SizeY) * (maxY - minY) + minY;

	return FVector(xReprojected, yReprojected, 0.0f);
}