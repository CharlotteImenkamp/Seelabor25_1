// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "LidarPointCloud.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCloudActor.generated.h"

class ULidarPointCloud;
class ULidarPointCloudComponent;

UCLASS()
class APointCloudActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointCloudActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//UFUNCTION(BlueprintCallable)
	//	void SetVisibility(bool visibility, FVector center, FVector extend);

	//UPROPERTY(BlueprintReadWrite)
	//	bool visible;

	//UPROPERTY(BlueprinReadWrite)
	//ALidarPointCloudActor* TestPointCloudActor; 

private:
	//TArray<FLidarPointCloudPoint> GeneratePointCloud(std::vector<FVector>* Location3D, FColor color, double zfactor, FVector scale, FVector translate);

	//TArray<FLidarPointCloudPoint> Points;
	//ULidarPointCloudComponent* pCCA;
	//ULidarPointCloudComponent* pCCB;
	//TArray<FString> path;

};
