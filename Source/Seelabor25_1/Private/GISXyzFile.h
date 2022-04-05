// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GISDataFile.h"
#include "GISXyzFile.generated.h"

UCLASS()
class UGISXyzFile : public UGISDataFile
{
	GENERATED_BODY()

public:
	UGISXyzFile();
	void Initialize(FString path, float minX, float maxX, float minY, float maxY, float cloudSizeX, float cloudSizeY);
	TArray<FVector> CalculateCoordinates(FString path);
	TArray<float> ExtractDepth();
	TArray<float> GetDepth();
	FString GetType();
	TArray<FString> GetAttributes(FString AttributeName) override { return TArray<FString>();  };

private:
	bool getFileContent(FString fileName, TArray<FVector>& vecOfFloat);

private:
	TArray<float> depth; 
};
