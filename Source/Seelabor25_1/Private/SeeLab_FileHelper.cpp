#include "SeeLab_FileHelper.h"

#include "CloudData.h"
#include "FileHelpers.h"




// Sets default values
ASeeLab_FileHelper::ASeeLab_FileHelper()
{
	PrimaryActorTick.bCanEverTick = false;



}


//-------------------------------------------------------------------------------------------------------------
// Load
//-------------------------------------------------------------------------------------------------------------

TArray<FVector> ASeeLab_FileHelper::LoadTxtToPoints(FString path)
{

	return TArray<FVector>();
}


//-------------------------------------------------------------------------------------------------------------
// Save
//-------------------------------------------------------------------------------------------------------------

void ASeeLab_FileHelper::SaveCloudData(FString path, UCloudData* data)
{
}

void ASeeLab_FileHelper::SaveXYZ(FString path, TArray<FVector> points)
{
}


void ASeeLab_FileHelper::SaveXyzRgb(FString path, TArray<FVector> points, FColor color)
{
}


void ASeeLab_FileHelper::SaveXyzRgb_arr(FString path, TArray<FVector> points, TArray<FColor> color)
{
}

int ASeeLab_FileHelper::GetNPointsInSphere(TArray<FVector2D> locations, FVector2D center, float range)
{
	int nPoints = 0; 
	float dist;

	for (FVector2D v : locations) {
		dist = FVector2D::Distance(v, center); 
		if (abs(dist) < range) {
			nPoints++; 
		}
	}
	return nPoints;
}

// Called when the game starts or when spawned
void ASeeLab_FileHelper::BeginPlay()
{
	Super::BeginPlay();
	
}




