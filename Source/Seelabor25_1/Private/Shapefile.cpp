// Fill out your copyright notice in the Description page of Project Settings.


#include "Shapefile.h"

#include "UnrealGDAL.h"
#include "GDALHelpers.h"

/*--------------------------------------------------------------------------
	CONSTRUCTION
----------------------------------------------------------------------------*/

Shapefile::Shapefile(){}
Shapefile::~Shapefile() {
}

Shapefile::Shapefile(FString path)
{
	// load 
	data = GDALHelpers::OpenVector(path, false);
	layers = GetLayers(GDALHelpers::OpenVector(path, false));

	// transform
	coordinates = CalCoordinates(data->GetLayer(0));
	CalculateAttributes(data->GetLayer(0)); 
}

void Shapefile::CalculateAttributes(OGRLayer* poLayer) {
	OGRFeature* feature = poLayer->GetFeature(0);
	int nFields = feature->GetDefnRef()->GetFieldCount();
	for (size_t iField = 0; iField < nFields; iField++)
	{
		OGRFieldDefn* defn = feature->GetFieldDefnRef(iField);
		attributes.push_back(defn->GetNameRef());
	}
}

std::vector<OGRLayer*> Shapefile::GetLayers(GDALDatasetRef poDS)
{
	// total number of layers
	int nLayer = poDS->GetLayerCount();

	// save layers in vector
	std::vector<OGRLayer*> poLayer(nLayer);
	for (int32 i = 0; i < poDS->GetLayerCount(); i++)
		poLayer[i] = poDS->GetLayer(i);

	return poLayer;
}

std::vector<FVector> Shapefile::CalCoordinates(OGRLayer* poLayer) 
{
	// parameters
	int nFeatures = poLayer->GetFeatureCount(); 
	std::vector<FVector> dataPoints(nFeatures); 

	// start from beginning
	poLayer->ResetReading();

	OGRwkbGeometryType poGeomType = wkbFlatten(poLayer->GetGeomType());
	; 

	// check type of feature (point, polygon or line)
	if (poGeomType == wkbPoint) {
		dataPoints = CalCooFromPointGeom(poLayer);
		Type = "Point"; 
	}
	else if (poGeomType == wkbLineString) {
		dataPoints = CalCooFromLineGeom(poLayer);
		Type = "Line"; 
	}
	else if (poGeomType == wkbMultiPolygon) {
		dataPoints = CalCooFromMultiPolygonGeom(poLayer);
		Type = "MultiPolygon"; 
	}
	else if (poGeomType == wkbPolygon) {
		dataPoints = CalCooFromPolygonGeom(poLayer);
		Type = "Polygon"; 
	}
	else	
		printf("no fitting geometry type found\n");
		
	return dataPoints; 
}





/*--------------------------------------------------------------------------
	COORDINATE TRANSFORMATION	
----------------------------------------------------------------------------*/

std::vector<FVector> Shapefile::CalCooFromPointGeom(OGRLayer* poLayer)
{
	int nFeature = poLayer->GetFeatureCount();
	std::vector<FVector> points(nFeature);
	OGRSpatialReference oSourceSRS = *(poLayer->GetSpatialRef());

	OGRGeometry* poGeometry;
	OGRPoint* poPoint; 

	// loop through all features
	for (GIntBig iFeature = 0; iFeature < nFeature; iFeature++)
	{
		poGeometry = poLayer->GetFeature(iFeature)->GetGeomFieldRef(0);
		poPoint = poGeometry->toPoint();

		points[iFeature] = FVector(poPoint->getX(), poPoint->getY(), poPoint->getZ());
	}

	ReprojectCoordinates(oSourceSRS, &points); 

	return points;
}

std::vector<FVector> Shapefile::CalCooFromLineGeom(OGRLayer* poLayer)
{
	OGRGeometry* poGeometry;
	OGRSpatialReference oSourceSRS = *(poLayer->GetSpatialRef());
	OGRLineString* poLineString;
	OGRPoint ptCurrentTemp;
	OGRPoint ptPreviousTemp; 

	int nFeature = poLayer->GetFeatureCount();
	std::vector<FVector> points;
	
	// loop through all features
	for (GIntBig iFeature = 0; iFeature < nFeature; iFeature++)
	{
		poGeometry = poLayer->GetFeature(iFeature)->GetGeomFieldRef(0);
		poLineString = poGeometry->toLineString();

		int nVert = poLineString->getNumPoints();
		for (int iVert = 0; iVert < nVert; iVert++)
		{
			poLineString->getPoint(iVert, &ptCurrentTemp);
			ReprojectCoordinates(oSourceSRS, &ptCurrentTemp);

			// skip first and last point
			if (iVert != 0 && iVert != nVert - 1) {
				poLineString->getPoint(iVert - 1, &ptPreviousTemp); 
				ReprojectCoordinates(oSourceSRS, &ptPreviousTemp); 
				std::vector<FVector> add = MakePointsInLine(ptPreviousTemp, ptCurrentTemp); 
				points.insert(points.end(), add.begin(), add.end()); 
			}
			points.push_back(FVector(ptCurrentTemp.getX(), ptCurrentTemp.getY(), ptCurrentTemp.getZ()));
		}
	}
	return points;
}

std::vector<FVector> Shapefile::CalCooFromPolygonGeom(OGRLayer* poLayer)
{
	OGRPolygon* poPolygon;
	OGRGeometry* poGeometry;
	OGRSpatialReference oSourceSRS = *(poLayer->GetSpatialRef());
	OGRLinearRing* poExteriorRing; 
	OGRPoint ptTemp, ptPrevTemp;

	int nInteriorRings, nExteriorRingVertices;
	int nFeature = poLayer->GetFeatureCount();
	std::vector<FVector> points;
	std::vector<FVector> polygonPoints; 

	// loop through all features
	for (GIntBig iFeature = 0; iFeature < nFeature; iFeature++)
	{
		poGeometry = poLayer->GetFeature(iFeature)->GetGeometryRef();
		poPolygon = (OGRPolygon*)poGeometry;

		// Exterior rings
		poExteriorRing = poPolygon->getExteriorRing();
		nExteriorRingVertices = poExteriorRing->getNumPoints();
		polygonPoints.clear(); 
		for (int iExteriorRingVertices = 0; iExteriorRingVertices < nExteriorRingVertices; iExteriorRingVertices++)
		{
			poExteriorRing->getPoint(iExteriorRingVertices, &ptTemp);
			ReprojectCoordinates(oSourceSRS, &ptTemp);


			 //connect ring line
			 //skip first and last point
			if (iExteriorRingVertices != 0 && iExteriorRingVertices != nExteriorRingVertices - 1) {
				poExteriorRing->getPoint(iExteriorRingVertices - 1, &ptPrevTemp);
				ReprojectCoordinates(oSourceSRS, &ptPrevTemp);
				 std::vector<FVector> add = MakePointsInLine(ptPrevTemp, ptTemp);
				 polygonPoints.insert(polygonPoints.end(), add.begin(), add.end());
			}
			// connect first and last point
			else if (iExteriorRingVertices == 0) {
				poExteriorRing->getPoint(nExteriorRingVertices-2, &ptPrevTemp);
				ReprojectCoordinates(oSourceSRS, &ptPrevTemp);
				 std::vector<FVector> add = MakePointsInLine(ptPrevTemp, ptTemp);
				 polygonPoints.insert(polygonPoints.end(), add.begin(), add.end());
			}

			polygonPoints.push_back(FVector(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ()));
		}

		// Interior Rings
		nInteriorRings = poPolygon->getNumInteriorRings();
		for (int h = 1; h <= nInteriorRings; h++)
		{
			OGRLinearRing* poInteriorRing = poPolygon->getInteriorRing(h - 1);
			int NumberOfInteriorRingVertices = poInteriorRing->getNumPoints();
			for (int k = 0; k < NumberOfInteriorRingVertices; k++)
			{
				poInteriorRing->getPoint(k, &ptTemp);
				ReprojectCoordinates(oSourceSRS, &ptTemp);
				polygonPoints.push_back(FVector(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ()));
			}
		}	
		points.insert(points.end(), polygonPoints.begin(), polygonPoints.end());
	}
	/*std::vector<FVector> add = MakePointsInPolygon(points);
	points.insert(points.end(), add.begin(), add.end());*/
	

	return points;
}

std::vector<FVector> Shapefile::CalCooFromMultiPolygonGeom(OGRLayer* poLayer)
{
	OGRGeometry* poGeometry, * poPolygonGeometry;
	OGRSpatialReference oSourceSRS = *(poLayer->GetSpatialRef());

	OGRPolygon* poPolygon;
	OGRMultiPolygon* poMultiPolygon;
	OGRLinearRing* poExteriorRing, * poInteriorRing;

	OGRPoint ptTemp;

	int nExteriorRingVertices, nInteriorRingVertices;
	int nInteriorRing;
	int nFeature = poLayer->GetFeatureCount();
	std::vector<FVector> points;

	// loop through all features
	for (GIntBig iFeature = 0; iFeature < nFeature; iFeature++)
	{
		poGeometry = poLayer->GetFeature(iFeature)->GetGeometryRef();
		poMultiPolygon = (OGRMultiPolygon*)poGeometry;
		int nGeometries = poMultiPolygon->getNumGeometries();

		// geometries
		for (int iGeometries = 0; iGeometries < nGeometries; iGeometries++)
		{
			poPolygonGeometry = poMultiPolygon->getGeometryRef(iGeometries);
			poPolygon = (OGRPolygon*)poPolygonGeometry;

			poExteriorRing = poPolygon->getExteriorRing();

			// exterior ring vertices
			nExteriorRingVertices = poExteriorRing->getNumPoints();
			for (int iExteriorRingVertices = 0; iExteriorRingVertices < nExteriorRingVertices; iExteriorRingVertices++)
			{
				poExteriorRing->getPoint(iExteriorRingVertices, &ptTemp);
				ReprojectCoordinates(oSourceSRS, &ptTemp); 
				points.push_back(FVector(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ()));
			}

			// interior rings
			nInteriorRing = poPolygon->getNumInteriorRings();
			for (int iInteriorRing = 0; iInteriorRing < nInteriorRing; iInteriorRing++)
			{
				poInteriorRing = poPolygon->getInteriorRing(iInteriorRing);

				// interior ring vertices
				nInteriorRingVertices = poInteriorRing->getNumPoints();
				for (int iInteriorRingVertices = 0; iInteriorRingVertices < nInteriorRingVertices; iInteriorRingVertices++)
				{
					poInteriorRing->getPoint(iInteriorRingVertices, &ptTemp);
					ReprojectCoordinates(oSourceSRS, &ptTemp);
					points.push_back(FVector(ptTemp.getX(), ptTemp.getY(), ptTemp.getZ()));
				}
			}
		}
	}
	return points;
}

/*--------------------------------------------------------------------------
	PROJECTION
----------------------------------------------------------------------------*/
void Shapefile::ReprojectCoordinates(OGRSpatialReference oSourceSRS, std::vector<FVector>* pointRef) {
	OGRSpatialReference oTargetSRS;
	oTargetSRS.SetWellKnownGeogCS("WGS84");

	if (oTargetSRS.IsSame(&oSourceSRS) == 0) {
		int nCoo = (*pointRef).size();
		std::vector<double> x(nCoo), y(nCoo), z(nCoo);

		OGRCoordinateTransformation* transform = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
		for (size_t i = 0; i < nCoo; i++)
		{
			x[i] = isinf((double)((*pointRef)[i].X)) ? 0.0 : (double)((*pointRef)[i].X);
			y[i] = isinf((double)((*pointRef)[i].Y)) ? 0.0 : (double)((*pointRef)[i].Y);
			z[i] = isinf((double)((*pointRef)[i].Z)) ? 0.0 : (double)((*pointRef)[i].Z);
		}
		int success = transform->Transform(nCoo, &x[0], &y[0], &z[0]);

		if (success == 1) {
			for (size_t i = 0; i < (*pointRef).size(); i++)
			{
				(*pointRef)[i] = FVector(x[i], y[i], z[i]);
			}
		}

	}
}
void Shapefile::ReprojectCoordinates(OGRSpatialReference oSourceSRS, OGRPoint* pointRef) {
	OGRSpatialReference oTargetSRS;
	oTargetSRS.SetWellKnownGeogCS("WGS84");

	if (oTargetSRS.IsSame(&oSourceSRS) == 0) {
		double x, y, z;

		OGRCoordinateTransformation* transform = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
		x = isinf((double)(*pointRef).getX()) ? 0.0 : (double)(*pointRef).getX();
		y = isinf((double)(*pointRef).getY()) ? 0.0 : (double)(*pointRef).getY();
		z = isinf((double)(*pointRef).getZ()) ? 0.0 : (double)(*pointRef).getZ();
	
		int success = transform->Transform(1, &x, &y, &z);

		if (success == 1) {
			(*pointRef) = OGRPoint(x, y, z);
		}

	}
}

TArray<FVector2D> Shapefile::GetImageCoordinates(float sizeX, float sizeY)
{
	TArray<FVector2D> imageCoordinates;

	std::vector<float> xCoordinates, yCoordinates;

	for (FVector p : coordinates) {
		xCoordinates.push_back(p.X);
		yCoordinates.push_back(p.Y);
	}

	float  x, y;
	std::pair<float, float> minmaxX(9.71324f, 10.23494f); //std::minmax_element(std::begin(xCoordinates), std::end(xCoordinates)); 
	std::pair<float, float> minmaxY(54.57100f, 54.39195f);  //std::minmax_element(std::begin(yCoordinates), std::end(yCoordinates));

	for (size_t i = 0; i < coordinates.size(); i++)
	{
		x = (xCoordinates[i] - minmaxX.first) / (minmaxX.second -minmaxX.first) * sizeX;
		y = - (yCoordinates[i] - minmaxY.first) / (minmaxY.second - minmaxY.first) * sizeY;
		imageCoordinates.Emplace(FVector2D(x, y));
	}

	return imageCoordinates;
}

std::string Shapefile::GetType()
{
	return Type;
}

void Shapefile::ReprojectCoordinates(OGRLayer* poLayer, std::vector<FVector>* cooRef)
{
	// Original Projection: feature -> geometry reference -> spatial reference
	OGRSpatialReference oSourceSRS; 
	OGRSpatialReference oTargetSRS;

	int nCoo;

	oSourceSRS = *(poLayer->GetSpatialRef());
	oTargetSRS.SetWellKnownGeogCS("WGS84");

	nCoo = (* cooRef).size();
	std::vector<double> x(nCoo), y(nCoo), z(nCoo);
	
	// create transform
	// if (oTargetSRS.IsSame(&oSourceSRS) == 0)
	// {
		OGRCoordinateTransformation* transform = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
		for (size_t i = 0; i < nCoo; i++)
		{
			x[i] = isinf((double)((*cooRef)[i].X))? 0.0: (double)((*cooRef)[i].X);
			y[i] = isinf((double)((*cooRef)[i].Y)) ? 0.0 : (double)((*cooRef)[i].Y);
			z[i] = isinf((double)((*cooRef)[i].Z)) ? 0.0 : (double)((*cooRef)[i].Z);
		}
		int success = transform->Transform(nCoo, &x[0], &y[0], &z[0]);
		
		if (success==1) {
			for (size_t i = 0; i < (*cooRef).size(); i++)
			{
				(*cooRef)[i] = FVector(x[i], y[i], z[i]);
			}
		}
		else {
			
		}
	// }
}

OGRLayer* Shapefile::ReprojectLayer(OGRLayer* inLayer) {
	OGRLayer* resLayer; 
	resLayer = inLayer; 
	
	// Original Projection: feature -> geometry reference -> spatial reference
	OGRSpatialReference oSourceSRS;
	OGRSpatialReference oTargetSRS;

	FString target = GDALHelpers::WktFromEPSG(4326);
	
	// FString source = inLayer->GetSpatialRef(); 


	oSourceSRS = *(inLayer->GetSpatialRef());
	//oTargetSRS.SetWellKnownGeogCS("WGS84");
	FString source = GDALHelpers::WktFromEPSG(32632);

	OGRCoordinateTransformationRef trans = GDALHelpers::CreateCoordinateTransform(source, target); 

	if (target != source) {
	}
	// create transform
	//if (oTargetSRS.IsSame(&oSourceSRS) == 0)
	//{
	//	OGRCoordinateTransformation* coordTrans = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
	//	OGRFeatureDefn* inLayerDefn; 

	//	// Output layer
	//	OGRLayer* outLayer = inLayer;

	//	// Add fields
	//	inLayerDefn = inLayer->GetLayerDefn();
	//	int nFieldCount = inLayerDefn->GetFieldCount(); 
	//	for (size_t iFieldCount = 0; iFieldCount < nFieldCount; iFieldCount++)
	//	{
	//		OGRFieldDefn* fieldDefn = inLayerDefn->GetFieldDefn(iFieldCount);
	//		outLayer->CreateField(fieldDefn); 

	//		OGRFeatureDefn * outLayerDefn = outLayer->GetLayerDefn(); 
	//		OGRFeature* inFeature = inLayer->GetNextFeature(); 

	//		while (inFeature) {
	//			// get the input geometry
	//			OGRGeometry* geom = inFeature->GetGeometryRef(); 

	//			// reproject the geometry
	//			geom->transform(coordTrans); 
	//			//create a new feature
	//			OGRFeature* outFeature = OGRFeature::CreateFeature(outLayerDefn); 
	//			//set the geometry and attribute
	//			outFeature->SetGeometry(geom); 
	//			for (size_t iField = 0; iField < outLayerDefn->GetFieldCount(); iField++)
	//			{
	//				// outFeature->SetField(outLayerDefn->GetFieldDefn(iField)->GetNameRef(), inFeature->GetField(iField));
	//				OGRFieldType inFieldType = inLayerDefn->GetFieldDefn(iField)->GetType(); 
	//				switch (inFieldType)
	//				{
	//				case OGRFieldType::OFTInteger:
	//					inFeature->GetFieldAsInteger(iField); 
	//					break; 
	//				case OGRFieldType::OFTString:
	//					inFeature->GetFieldAsString(iField); 
	//					//\Todo
	//				default:
	//					break;
	//				}
	//					
	//				//add the feature to the shapefile
	//				// outLayer->CreateFeature(outFeature); 
	//			}
	//			inFeature = inLayer->GetNextFeature();
	//		}
	//	}
	//	resLayer = outLayer; 
	//}

	return resLayer;
}

/*--------------------------------------------------------------------------
	HELPER
----------------------------------------------------------------------------*/

FVector Shapefile::ConvertToVector(OGRPoint po) {
	return FVector(po.getX(), po.getY(), po.getZ());
}

std::vector<FVector> Shapefile::MakePointsInPolygon(std::vector<FVector> polygon)
{
	float xmin=polygon[0].X, xmax=polygon[0].X, ymin=polygon[0].Y, ymax= polygon[0].Y;
	float stepsize = 0.001; 
	std::vector<FVector> pointsInPolygon; 

	for (FVector v : polygon) {
		if (v.X < xmin)xmin = v.X; 
		if (v.X > xmax)xmax = v.X; 
		if (v.Y < ymin)ymin = v.Y; 
		if (v.Y > ymax)ymax = v.Y; 
	}

	float iX = xmin;
	float iY = ymin; 
	while (iX < xmax)
	{
		iY = ymin; 
		while (iY < ymax) {
			FVector p = FVector(iX, iY, 0.0f); 
			if (pnpoly(polygon, p)) {
				pointsInPolygon.push_back(p); 
			}
			iY += stepsize; 
		}
		iX += stepsize; 
	}
	return pointsInPolygon; 
}

bool Shapefile::pnpoly(std::vector<FVector> vert, FVector p)
{
	int nvert = vert.size(); 
	bool inside = false; 

	int i, j;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((vert[i].Y > p.Y) != (vert[j].Y > p.Y)) &&
			(p.X < (vert[j].X - vert[i].X) * (p.Y - vert[i].Y) / (vert[j].Y - vert[i].Y) + vert[i].X))
			inside = !inside;
	}
	return inside; 
}



std::vector<FVector> Shapefile::MakePointsInLine(OGRPoint start, OGRPoint end) {
	std::vector<FVector> line;
	FVector po = ConvertToVector(start);
	double stepsize = 0.0001;
	FVector direction(ConvertToVector(end) - ConvertToVector(start));
	// direction = FVector(direction.X/abs(direction.X), direction.Y/abs(direction.Y), direction.Z/abs(direction.Z));
	direction.Normalize(); 

	int nIterations = 0; 
	while (!(FVector::Distance(po, ConvertToVector(end)) < 2 * stepsize) && nIterations <10000 )
	{
		po = po + stepsize * direction;
		line.push_back(po);
		nIterations++; 
	}
	return line;
}

TArray<FString> Shapefile::GetAttributes()
{
	
	return ConvertToUnrealArray(attributes);
}

template <typename T>
TArray<T> Shapefile::ConvertToUnrealArray(std::vector<T> v)
{
	TArray<T> a;
	a.SetNumUninitialized(v.size());

	for (int i = 0; i < v.size(); i++) {
		a[i] = v[i];
	}
	return a;
}
