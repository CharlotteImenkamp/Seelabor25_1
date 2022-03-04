// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SpawnActor.h"
#include "Templates/Tuple.h"
#include "DataManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LayerManager.generated.h"

UCLASS()
class SEELABOR25_1_API ALayerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALayerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void SetInvisible(); 
	
	UFUNCTION(BlueprintCallable)
		void SetVisible();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: 
	ASpawnActor* test;

};
