// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Shapefile.h"

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
		void Initialize(FString path, FString name, FColor color);

	UFUNCTION(BlueprintCallable)
		void ExportPoints();

	UFUNCTION(BlueprintCallable)
		TArray<FVector> GetReducedPointSet(); 



public:	
	UPROPERTY(BlueprintReadWrite)
		TArray<FString> Attributes; 

	UPROPERTY(BlueprintReadWrite)
		TArray<FVector2D> Locations2D;

	UPROPERTY(BlueprintReadWrite)
		FString Type;

	UPROPERTY(BlueprintReadWrite)
		FString Name;

	UPROPERTY(BlueprintReadWrite)
		FColor Color;

	UPROPERTY(BlueprintReadWrite)
		float Size;
		
};


