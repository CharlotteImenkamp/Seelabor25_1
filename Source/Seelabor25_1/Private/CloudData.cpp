

#include "CloudData.h"

#include "Shapefile.h"

// Sets default values for this component's properties
UCloudData::UCloudData()
{
	Locations2D = {};
	Attributes = {}; 
	Type = "";
	Name = "";
	Color = FColor::White;
	Size = 1.0f;
}

UCloudData::~UCloudData() {
	Locations2D.Empty();
}

void UCloudData::Initialize(FString path, FString name, FColor color)
{
	Shapefile shp(path);
	Locations2D = shp.GetImageCoordinates(1920.0f, 1080.0f);
	// funktioniert nicht ** Attributes = shp.GetAttributes(); 
	Type = shp.GetType().c_str();
	Color = color;
	Name = name;
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
	int max = 20; 

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



