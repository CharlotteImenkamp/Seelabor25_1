// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GISDataFile.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SEELABOR25_1_API UGISDataFile : public UObject
{
	GENERATED_BODY()

public: 
	TArray<FVector2D> GetImageCoordinates();

	static FVector GetWorldCoordinate(float x, float y, float SizeX, float SizeY, float minX, float maxX, float minY, float maxY);
	static FVector GetEPSG(float x, float y, float SizeX, float SizeY, float minX, float maxX, float minY, float maxY);

	UFUNCTION(BlueprintCallable)
		virtual TArray<FString> GetAttributes(FString AttributeName);

protected: 
	virtual TArray<FVector> CalculateCoordinates(FString path);

// parameters
	std::pair<float, float> minmaxX;
	std::pair<float, float> minmaxY;
	float SizeX;
	float SizeY;
	TArray<FVector> coordinates;
	std::string Type;
};
