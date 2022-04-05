// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealGDAL.h"
#include "GDALHelpers.h"


#include "CoreMinimal.h"
#include "GISDataFile.h"
#include "GISRasterfile.generated.h"

UCLASS()
class SEELABOR25_1_API UGISRasterfile : public UGISDataFile
{
	GENERATED_BODY()



public:
	UGISRasterfile(){};
	~UGISRasterfile() {}; 
	
	TArray<int> GetHeight(FString path, FVector2D& size);

};
