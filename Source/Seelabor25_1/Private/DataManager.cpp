// Fill out your copyright notice in the Description page of Project Settings.


#include "DataManager.h"

#include "LidarPointCloud.h"
#include "LidarPointCloudComponent.h"

// Sets default values
ADataManager::ADataManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// point*** funktioniert
	Initialization.Emplace(DataInit("Baltzos", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Habitate//Biodiversitaet//Makrophyten//BALTZOS_2018_20_Schubert_EckB.shp", FColor::Blue));
	Initialization.Emplace(DataInit("Fucus", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Habitate//Biodiversitaet//Makrophyten//EckB_trans_foto_2014_2015_Ivo.shp", FColor::Black));
	Initialization.Emplace(DataInit("Cities", "C://Users//IGD//1-Seelabor//02-Visualisierung//Daten//Shapefile_Eckernfoerde//shape//points.shp", FColor::Red));
	Initialization.Emplace(DataInit("Places", "C://Users//IGD//1-Seelabor//02-Visualisierung//Daten//Shapefile_Eckernfoerde//shape//places.shp", FColor::Green));
	Initialization.Emplace(DataInit("Einleitung Klaera.", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Einleitung//Clip_Einleitstellen_Klaeranlagen.shp", FColor::Yellow));
	Initialization.Emplace(DataInit("Einleitung sonstige", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Einleitung//Clip_Einleitstellen_sonstige gewerbliche.shp", FColor::Cyan)); 
	//*** Fehler*** kein EPSG Initialization.Emplace(ShapefileInit("Niederschlag", FColor::Orange, 1.0f, "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Einleitung//Eckerfoerde_Niederschlag.shp"));
	Initialization.Emplace(DataInit("Ankerplaetze", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Ankerplaetze.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Baden", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Badestellen.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Camping", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Campingplaetze_Punkt.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Hafen", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Hafen.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Kite", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Kiteplaetze.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Tauchen", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Tauchen.shp", FColor::Magenta));
	Initialization.Emplace(DataInit("Wassersport", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Tourismus_Freizeitaktivitaeten//Wassersport Schulen.shp", FColor::Magenta));
	
	//line funktioniert
	Initialization.Emplace(DataInit("Railways", "C://Users//IGD//1-Seelabor//02-Visualisierung//Daten//Shapefile_Eckernfoerde//shape//railways.shp", FColor::Blue));
	Initialization.Emplace(DataInit("Roads", "C://Users//IGD//1-Seelabor//02-Visualisierung//Daten//Shapefile_Eckernfoerde//shape//roads.shp", FColor::Blue));
	Initialization.Emplace(DataInit("Waterways", "C://Users//IGD//1-Seelabor//02-Visualisierung//Daten//Shapefile_Eckernfoerde//shape//waterways.shp", FColor::Blue)); 

	// multiPolygon
	Initialization.Emplace(DataInit("Sperrgebiete", "C://Users//IGD//1-Seelabor//02-Visualisierung//Upload BioC//Nutzungen//Sperrgebiete//Sperrgebiet Boknis Eck.shp", FColor::Emerald));
}

void ADataManager::Initialize() 
{
	for (size_t i = 0; i < Initialization.Num(); i++)
	{
		data.Add(NewObject<UCloudData>());
		data[i]->Initialize(Initialization[i].Path, Initialization[i].Name, Initialization[i].Color);
		//data[i]->ExportPoints(); 
	}
}

// Called when the game starts or when spawned
void ADataManager::BeginPlay()
{
	Super::BeginPlay();
	Initialize(); 
}

// Called every frame
void ADataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

