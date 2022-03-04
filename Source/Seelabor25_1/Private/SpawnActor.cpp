
#include "SpawnActor.h"
#include "CloudData.h"

#include "LidarPointCloud.h"
#include "LidarPointCloudComponent.h"
#include "DataManager.h"
#include "Shapefile.h"
#include "Misc/FileHelper.h"

ASpawnActor::ASpawnActor(){
	// Set Default Parameter
	Color = FColor::White; 
	PointSize = 5.0f;
	PointShape = ELidarPointCloudSpriteShape::Circle;
	IsVisible = true;
	CloudName = "Default"; 
	NDataPoints = 0; 
	Attributes = {}; 
}

void ASpawnActor::AddPoints(UCloudData*data) {
	points.Append(GeneratePointCloud(data->Locations2D, data->Color)); 
	// this->SetPointCloud(ULidarPointCloud::CreateFromData(points, true)); 
}

void ASpawnActor::Initialize(UCloudData* data)
{
	// Set parameters
	Color = data->Color; 
	CloudName = data->Name; 
	PointSize = data->Size; 

	points = GeneratePointCloud(data->Locations2D, data->Color); 

	//// Set Cloud Parameter
	//this->SetPointCloud(ULidarPointCloud::CreateFromData(points, true));
	//this->GetPointCloudComponent()->PointShape = PointShape;
	//this->GetPointCloudComponent()->PointSize = PointSize;
	//this->GetPointCloudComponent()->SetVisibility(IsVisible);

	NDataPoints = data->Locations2D.Num(); 

	////\todo move to function 
	//std::vector<FString> attr = shp.GetAttributes();
	//for (size_t i = 0; i < attr.size(); i++)
	//{
	//	Attributes.Emplace(attr[i]); 
	//}

	// save pointcloud to file
	// ExportPointCloud(CloudName); 
}

ASpawnActor::~ASpawnActor()
{
}

void ASpawnActor::ChangeColor(FColor newColor)
{
}

void ASpawnActor::ChangePointSize(float newSize)
{
	PointSize = newSize; 
	this->GetPointCloudComponent()->PointSize = PointSize;
}

void ASpawnActor::ChangeVisibility(bool newVisibility)
{
	IsVisible = newVisibility;
	this->GetPointCloudComponent()->SetVisibility(IsVisible);
}

void ASpawnActor::ChangeShape(ELidarPointCloudSpriteShape pointShape)
{
	PointShape = pointShape; 
	this->GetPointCloudComponent()->PointShape = PointShape;
}

void ASpawnActor::ExportPointCloud(FString name)
{
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir() + TEXT("/PointCloud" + name + ".txt"));
	FString FileContent = TEXT("X,Y,Z,R,G,B");

	double i = 1.0;
	
	for(FLidarPointCloudPoint p :points)
	{
		
		FileContent += FString::Printf(TEXT("\n%f,%f,%f,%i,%i,%i"), p.Location.X, p.Location.Y, p.Location.Z, p.Color.R, p.Color.G, p.Color.B);
		i++; 
	}

	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get()); //EFileWrite::FILEWRITE_Append);

}

void ASpawnActor::GetAttributes()
{
}

void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASpawnActor::ManipulatePoints(std::vector<FVector>* Coordinates, FVector scale, FVector translate, double zfactor) {

	double x, y, z; 
	for (size_t i = 0; i < Coordinates->size(); i++)
	{
		x = ((*Coordinates)[i].X - translate.X) * scale.X; 
		y = ((*Coordinates)[i].Y - translate.Y) * scale.Y; 
		z = ((*Coordinates)[i].Z - translate.Z) * scale.Z; 

		(*Coordinates)[i] = FVector(x, y, i*zfactor); 
	}
}

TArray<FLidarPointCloudPoint> ASpawnActor::GeneratePointCloud(TArray<FVector> Location3D, double zfactor, FVector scale, FVector translate)
{
	TArray<FLidarPointCloudPoint> point;

	// Populate the point array with points
	for (size_t i = 1; i < Location3D.Num(); i++)
	{
		point.Emplace(FLidarPointCloudPoint((Location3D)[i], Color, true, 1));
	}
	return point;
}

TArray<FLidarPointCloudPoint> ASpawnActor::GeneratePointCloud(TArray<FVector2D> Location3D, FColor color)
{
	TArray<FLidarPointCloudPoint> point;

	// Populate the point array with points
	for (size_t i = 1; i < Location3D.Num(); i++)
	{
		FVector v = FVector((Location3D)[i].X, (Location3D)[i].Y, i * 0.0001);
		point.Emplace(FLidarPointCloudPoint(v, color, true, 1));
	}
	return point;
}

// \TODO Für Rechteck: PointStart bei Mouse down und pointEnd bei Mouse up
TArray<FVector> ASpawnActor::GetPointsInCircle(FVector origin, float radius) {
	
	TArray<FLidarPointCloudPoint> resPointCloudPoints;
	TArray<FVector> resPoints; 

	resPointCloudPoints = this->GetPointCloud()->GetPointsInSphereAsCopies(origin, radius, false, true);

	for (FLidarPointCloudPoint p: resPointCloudPoints)
	{
		resPoints.Emplace(p.Location); 
	}

	return resPoints;
}