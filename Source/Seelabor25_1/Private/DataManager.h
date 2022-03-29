// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LidarPointCloud.h"
#include "LidarPointCloudComponent.h"
#include "CloudData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataManager.generated.h"

UCLASS()
class ADataManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULidarPointCloud* cloud; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UCloudData*> data;

	void Initialize();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	struct DataInit {

	public:
		DataInit(FString name, FString path, FColor color) {
			Name = name;
			Path = path;
			Color = color; 
		};
		FString Path;
		FString Name;
		FColor Color; 
	};

	TArray<DataInit> InitShapefile;
	TArray<DataInit> InitXYZFile; 
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};


