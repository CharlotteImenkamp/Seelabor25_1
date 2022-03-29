// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CloudData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeeLab_FileHelper.generated.h"

UCLASS()
class SEELABOR25_1_API ASeeLab_FileHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeeLab_FileHelper();

	//-------------------------------
	// Load
	//-------------------------------

	ASeeLab_FileHelper(FString path, float minX, float maxX, float minY, float maxY, float cloudSizeX, float cloudSizeY);

	FVector GetCoordinates(FString path);

	bool getFileContent(FString fileName, std::vector<std::string>& vecOfStrs);
	
	FVector2D GetImageCoordinates();
	FString GetType();
	static TArray<FVector> LoadTxtToPoints(const FString path);


	//-------------------------------
	// Save
	//-------------------------------

	UFUNCTION(BlueprintCallable)
		 void SaveCloudData(FString path, UCloudData* data);

	UFUNCTION(BlueprintCallable)
		 void SaveXYZ(FString path, TArray<FVector> points);

	UFUNCTION(BlueprintCallable)
		 void SaveXyzRgb(FString path, TArray<FVector> points, FColor color);

	// TODO
	UFUNCTION(BlueprintCallable)
		 void SaveXyzRgb_arr(FString path, TArray<FVector> points, TArray<FColor> color);

	UFUNCTION(BlueprintCallable)
		int GetNPointsInSphere(TArray<FVector2D> locations, FVector2D center, float range); 

protected:
	virtual void BeginPlay() override;

private: 




};
