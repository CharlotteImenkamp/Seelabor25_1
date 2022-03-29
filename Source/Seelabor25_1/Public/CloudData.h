// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GISDataFile.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloudData.generated.h"


UCLASS(BlueprintType)
class SEELABOR25_1_API UCloudData : public UObject
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCloudData();
	~UCloudData();


	UFUNCTION(BlueprintCallable)
		void InitializeShapefile(FString path, FString name, FColor color);

	UFUNCTION(BlueprintCallable)
		void InitializeXYZFile(FString path, FString name, FColor color); 

	UFUNCTION(BlueprintCallable)
		void ExportPoints();

	UFUNCTION(BlueprintCallable)
		TArray<FVector> GetReducedPointSet(); 

	UFUNCTION(BlueprintCallable)
		FVector GetWorldCoordinate(float x, float y); 

	UFUNCTION(BlueprintCallable)
		FVector GetEPSGCoordinate(float x, float y); 

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetAttribute(FString AttributeName); 

private: 
	float minX;
	float maxX;
	float minY;
	float maxY;

public:	
	UPROPERTY(BlueprintReadWrite)
		float CloudSizeX; 

	UPROPERTY(BlueprintReadWrite)
		float CloudSizeY;

	UPROPERTY(BlueprintReadWrite)
		bool depthAvailable; 

	UPROPERTY(BlueprintReadWrite)
		UGISDataFile* fileRef; 

	UPROPERTY(BlueprintReadWrite)
		TArray<float> depth; 


	UPROPERTY(BlueprintReadWrite)
		TArray<FVector2D> Locations2D;

	UPROPERTY(BlueprintReadWrite)
		FString Type;

	UPROPERTY(BlueprintReadWrite)
		FString Name;

	UPROPERTY(BlueprintReadWrite)
		FColor Color; 


		
};


