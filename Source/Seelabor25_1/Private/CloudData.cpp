
#include "CloudData.h"

#include "SeeLab_FileHelper.h"
#include "GISShapefile.h"
#include "GISXyzFile.h"

UCloudData::UCloudData()
{
	Locations2D = {};
	Type = "";
	Name = "";
	depthAvailable = false; 
	depth = {}; 

	minX = 9.71324f;
	maxX = 10.23494f;
	maxY = 54.39195f;
	minY = 54.57100f;

	CloudSizeX = 1920.0f;
	CloudSizeY = 1920.0f;
}

UCloudData::~UCloudData() {
	Locations2D.Empty();
}

void UCloudData::InitializeShapefile(FString path, FString name, FColor color)
{
	UGISShapefile* shp = NewObject<UGISShapefile>(); 
	fileRef = (UGISShapefile*)shp;
	shp->Initialize(path, minX, maxX, minY, maxY, CloudSizeX, CloudSizeY);
	Locations2D = shp->GetImageCoordinates();
	Color = color;
	Name = name;
	depthAvailable = false; 
}

void UCloudData::InitializeXYZFile(FString path, FString name, FColor color) {
	UGISXyzFile* xyz = NewObject<UGISXyzFile>(); 
	fileRef = (UGISDataFile*)xyz; 
	xyz->Initialize(path, minX, maxX, minY, maxY, CloudSizeX, CloudSizeY); 
	Locations2D = xyz->GetImageCoordinates(); 
	Type = xyz->GetType(); 
	Name = name; 
	depthAvailable = true; 
	depth = xyz->GetDepth(); 
}

void UCloudData::ExportPoints() {
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir() + TEXT("/PointCloud" + Name + ".txt"));
	FString FileContent = TEXT("X,Y,Z,R,G,B");

	double i = 1.0;

	for (FVector2D p : Locations2D)
	{
		FileContent += FString::Printf(TEXT("\n%f,%f,%f,%i,%i,%i"), p.X, p.Y, i * 0.0001, Color.R, Color.G, Color.B);
		i++;
	}
	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get()); //EFileWrite::FILEWRITE_Append);
}

TArray<FVector> UCloudData::GetReducedPointSet()
{
	TArray<FVector2D> temp; 
	int max = 1000; 

	if (Locations2D.Num() > max) {
		for (int i = 0; i < Locations2D.Num(); i=i+(Locations2D.Num()/max))
		{
			temp.Add(Locations2D[i]); 
		}
	}
	else {
		temp = Locations2D; 
	}

	TArray<FVector> res;

	// Convert to 3D
	for (FVector2D v:temp)
	{
		res.Add(FVector(v.X, v.Y, 0.0f)); 
	}

	return res; 
}

FVector UCloudData::GetWorldCoordinate(float x, float y)
{
	return UGISDataFile::GetWorldCoordinate(x,y, CloudSizeX, CloudSizeY, minX, maxX, minY, maxY);
}

FVector UCloudData::GetEPSGCoordinate(float x, float y) {
	return UGISDataFile::GetEPSG(x, y, CloudSizeX, CloudSizeY, minX, maxX, minY, maxY);
}

TArray<FString> UCloudData::GetAttribute(FString AttributeName)
{
	return fileRef->GetAttributes(AttributeName);
}





