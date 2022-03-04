// Fill out your copyright notice in the Description page of Project Settings.


#include "LayerManager.h"
#include "SpawnActor.h"
#include "DataManager.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ALayerManager::ALayerManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Spawn PointCloud Actors
	
	//TArray<ULidarPointCloud*> cPs; 
	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADataManager::StaticClass(), FoundActors);
	//auto dataManager = (ADataManager*) FoundActors[0]; 
	//dataManager->Initialize(); 

	// Add point cloud for each 
	//ASpawnActor* ref; 
	//for (size_t i = 0; i < dataManager->data.Num(); i++)
	//{
	//	ref = GetWorld()->SpawnActor<ASpawnActor>(ASpawnActor::StaticClass(), FTransform(FVector(0, 0, 0))); 
	//	ref->Initialize(dataManager->data[i]);
	//	pointCloudActorRefs.Emplace(ref);
	//	cPs.Emplace(ref->GetPointCloud()); 
	//}
	
	// add one point cloud 
	//test = GetWorld()->SpawnActor<ASpawnActor>(ASpawnActor::StaticClass(), FTransform(FVector(0, 0, 0)));
	//test->Initialize(dataManager->data[0]); 
	//for (size_t i = 1; i < dataManager->data.Num(); i++)
	//{
	//	test->AddPoints(dataManager->data[i]); 
	//}

	//pointCloudActorRefs.Emplace(test); 
	//test->ExportPointCloud("Complete"); 
}


void ALayerManager::SetInvisible()
{
	//for (ASpawnActor* a : pointCloudActorRefs)
	//	a->ChangeVisibility(false); 
}

void ALayerManager::SetVisible()
{
	//for (ASpawnActor* a : pointCloudActorRefs)
	//	a->ChangeVisibility(true);
}

// Called every frame
void ALayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
