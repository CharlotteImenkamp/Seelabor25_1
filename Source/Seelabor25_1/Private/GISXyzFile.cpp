#include "GISXyzFile.h"


#include <iostream>
#include <fstream>
#include <string>

UGISXyzFile::UGISXyzFile()
{
}

void UGISXyzFile::Initialize(FString path, float minX, float maxX, float minY, float maxY, float cloudSizeX, float cloudSizeY)
{
	minmaxX = { minX, maxX };
	minmaxY = { minY, maxY }; 	

	SizeX = cloudSizeX;
	SizeY = cloudSizeY;

	CalculateCoordinates(path);
	depth = ExtractDepth(); 
}

TArray<FVector> UGISXyzFile::CalculateCoordinates(FString path)
{
	std::vector<std::string> vecOfStrs;

	getFileContent(path, coordinates);

	return coordinates;
}

TArray<float> UGISXyzFile::GetDepth() {
	return depth; 
}

TArray<float> UGISXyzFile::ExtractDepth() 
{
	TArray<float> res; 
	for (FVector v:coordinates)
	{
		res.Add(v.Z); 
	}
	return res; 
}

bool UGISXyzFile::getFileContent(FString fileName, TArray<FVector>& vecOfFloat)
{
	std::ifstream in(*fileName);
	std::vector<std::string> vecOfStrs;

	std::string s;
	std::string substr;
	std::string delimiter = " ";
	size_t delim_len = delimiter.length();
	size_t pos_end = 0, pos_start;
	float x, y, z;
	while (std::getline(in, s))
	{
		pos_start = 0;
		vecOfStrs.clear();
		if (s.size() > 0) {
			while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
				substr = s.substr(pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				vecOfStrs.push_back(substr);
			}
			substr = s.substr(pos_start, s.length());
			vecOfStrs.push_back(substr);
			x = std::stof(vecOfStrs[0]);
			y = std::stof(vecOfStrs[1]);
			z = (std::stof(substr) != 9999.0f) ? std::stof(substr) : 0.0f;
			vecOfFloat.Add(FVector(x, y, z));
		}
	}
	in.close();

	return true;
}



FString UGISXyzFile::GetType() {

	return FString("XYZ");
}


