#include "PointCloudActor.h"

#include "Shapefile.h"
#include "LidarPointCloud.h"
#include "LidarPointCloudComponent.h"
//#include "LidarPointCloudActor.h"

// Sets default values
APointCloudActor::APointCloudActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APointCloudActor::BeginPlay()
{
	Super::BeginPlay();
}

