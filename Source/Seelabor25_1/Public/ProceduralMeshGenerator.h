// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProceduralMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshGenerator.generated.h"

UCLASS()
class SEELABOR25_1_API AProceduralMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMeshGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* RestrictedArea;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
		void AddSection(TArray<int32> height, int XLength, int YLength, float GridSize);

	UFUNCTION(BlueprintCallable)
		void GetPixelFromPNG(FString path, TArray<int32>& height, FVector2D& size);

	UFUNCTION(BlueprintCallable)
		FVector2D GetPixelFromUTexture(UTexture2D* myTexture2D, TArray<int32>& height);

	UFUNCTION(BlueprintCallable)
		void GenerateBathymetrieData();

	FColor MapDepthToColor(float depth, float range);
	float smoothstep(float edge0, float edge1, float x);

	UFUNCTION(BlueprintCallable)
		UProceduralMeshComponent* AddAreaMesh(TArray<FVector2D> Outline);

	FColor IsInside(FVector point, TArray<FVector2D>& points);

	int section; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
