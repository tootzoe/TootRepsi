// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointLightComponent.h"

#include "UObject/UnrealType.h"

#include "TootRepsi/PrjLog.h"




// Sets default values
AMapRoom::AMapRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    GridSize = 1000.f;
    RoomSize = 3;
    doRebuild = true;


    USceneComponent* tmpRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
    SetRootComponent(tmpRoot);

    //walls
    Walls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Walls"));
    Walls->SetupAttachment(tmpRoot);
    //edges
    Edges = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Edges"));
    Edges->SetupAttachment(tmpRoot);
    //corners
    Corners = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Corners"));
    Corners->SetupAttachment(tmpRoot);
    //tube walls
    TubeWalls = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TubeWalls"));
    TubeWalls->SetupAttachment(tmpRoot);
    //tubes
    Tubes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tubes"));
    Tubes->SetupAttachment(tmpRoot);


}

// Called when the game starts or when spawned
// void AMapRoom::BeginPlay()
// {
// 	Super::BeginPlay();
	
// }

// // Called every frame
// void AMapRoom::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }


static FORCEINLINE void addMeshInstance(UInstancedStaticMeshComponent* comp,
                                        const FRotator& rotSelf,
                                        const FRotator& rot,
                                        const FVector& tran)
{
    comp->AddInstance(FTransform(rotSelf, rot.RotateVector(tran)));

}


void AMapRoom::OnConstruction(const FTransform &Transform)
{
    AActor::OnConstruction(Transform);

    if(!doRebuild )
    {
        return;
    }

    doRebuild = false;

    //UE_LOG(LogTootRepsi, Warning, TEXT("Rebuild MapRoom....%hs") , __func__);


    lastGridSz =  GridSize;
    lastRoomSz  = RoomSize;

    Walls->ClearInstances();
    Edges->ClearInstances();
    Corners->ClearInstances();
    TubeWalls->ClearInstances();
    Tubes->ClearInstances();

 #if 0
    for(auto& l : pointArr)
        l->DestroyComponent();
    pointArr.Empty();
#else
    // another way get all same components
    TArray<UPointLightComponent*> tmpLightArr;
    GetComponents<UPointLightComponent>(tmpLightArr);
    for(auto l : tmpLightArr)
        l->DestroyComponent();
#endif



    int32 halfSz = RoomSize / 2;
    int32 wallOffset = (halfSz + 1) * GridSize;
    FVector tmpWallPos (-wallOffset , 0.f , 0.f);


    const TArray<FRotator> tmpRotatorArr = {
        FRotator(0.f , 0.f , 0.f),        // -x
        FRotator(0.f , 180.f , 0.f),      // +x
        FRotator(0.f , -90.f , 0.f),        // +y
        FRotator(0.f , 90.f , 0.f),      // -y
        FRotator( -90.f , 0.f , 0.f),        // +z
        FRotator( 90.f , 0.f , 0.f),      // -z
    };

    const TArray<uint32> tmpTubeCntPerSideArr = {  // *MUST* match the size of above array (tmpRotatorArr)
        NegativeXTubeCount,      // -x
        PositiveXTubeCount,  // +x
        PositiveYTubeCount,        // +y
        NegativeYTubeCount,      // -y
        PositiveZTubeCount,        // +z
        NegativeZTubeCount,      // -z
    };



    const TArray<FRotator> tmpEdgesRotatorArr = {
        FRotator(90.f , 0.f ,  0.f),
        FRotator(90.f , 180.f , 0.f),
        FRotator(90.f , -90.f , 0.f),
        FRotator(90.f , 90.f , 0.f),
    };

    const TArray<FRotator> tmpMidEdgesRotatorArr = {
        FRotator(0.f , 0.f , 90.f),
        FRotator(0.f , 180.f , 90.f),
        FRotator(0.f , -90.f , 90.f),
        FRotator(0.f , 90.f , 90.f),
    };


    // build walls
    for (int i  = - halfSz ; i  <= halfSz; ++i ) {
        tmpWallPos.Y = GridSize * i;

        for (int j = -halfSz ; j <= halfSz; ++j) {
            tmpWallPos.Z = GridSize * j;

            auto WallOrTubeWall = [&](uint32 tubeCnt){
                return (i == 0 && j == 0 && tubeCnt > 0) ? TubeWalls : Walls;
            };



            for ( FRotator  r : tmpRotatorArr) {
                //UE_LOG(LogTemp, Warning, TEXT("Idx %d ....%hs") , tmpRotatorArr.IndexOfByKey(r), __func__);
                 addMeshInstance(WallOrTubeWall(tmpTubeCntPerSideArr[tmpRotatorArr.IndexOfByKey(r)]), r, r , tmpWallPos );
            }
        }

        // build edges
        const TArray<int32> tmpTopAndBottom = { -1 , 0 , 1};
        for( int32 ii : tmpTopAndBottom){
            tmpWallPos.Z = wallOffset * ii;
            if( ii != 0 ){
                for (int jj = 0 ; jj < tmpEdgesRotatorArr.Num(); ++jj) {
                    addMeshInstance(Edges, ii == 1 ? tmpRotatorArr[jj] :  tmpEdgesRotatorArr[jj] ,tmpRotatorArr[jj] , tmpWallPos);
                }
            }else{
                tmpWallPos.Z = wallOffset ;
                for ( FRotator  r : tmpMidEdgesRotatorArr) {
                    addMeshInstance(Edges,r, r ,tmpWallPos) ;
                }
            }
        }
    }


    // build corner
    tmpWallPos.Y = tmpWallPos.Y + GridSize;
    for (int i = 0; i < 2; ++i) {
        if(i == 0){
            for(int j = 0 ; j < 4 ; j ++){
                addMeshInstance(Corners,tmpRotatorArr[j] ,tmpRotatorArr[j] , tmpWallPos);
            }
        }else{
            tmpWallPos.Z = -wallOffset;
            for(int j = 0 ; j < 4 ; j ++){
                addMeshInstance(Corners,tmpMidEdgesRotatorArr[j] ,tmpRotatorArr[j] , tmpWallPos);
            }
        }

    }

    // build tubes
    tmpWallPos.Y = 0;
    tmpWallPos.Z = 0;

    auto placeTubesByCount = [&](uint32 cnt , const FRotator &rot){
         for (uint32 i = 1; i < cnt; ++i) {
              tmpWallPos.X = -1 * (wallOffset + 525 + GridSize * i);
              addMeshInstance(Tubes, rot , rot , tmpWallPos);
              placePointLight(1.f , GridSize, rot, tmpWallPos);
         }
    };

    for ( FRotator  r : tmpRotatorArr) {
        //UE_LOG(LogTemp, Warning, TEXT("Idx %d ....%hs") , tmpRotatorArr.IndexOfByKey(r), __func__);
         placeTubesByCount(tmpTubeCntPerSideArr[tmpRotatorArr.IndexOfByKey(r)], r);
    }


    // place light
    tmpWallPos.X = 0;
    placePointLight(2.f, wallOffset * 2, tmpRotatorArr[0] , tmpWallPos);

}

#if WITH_EDITOR
void AMapRoom::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    if(PropertyChangedEvent.Property && PropertyChangedEvent.Property->HasMetaData("RebuildMapRoom"))
        doRebuild = true;

    Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


void AMapRoom::placePointLight(float intensity, float radius, const FRotator &rot, const FVector &translation)
{
    auto l =  NewObject<UPointLightComponent>(this, MakeUniqueObjectName(this, UPointLightComponent::StaticClass()));

    checkf(l , TEXT("Failed to create new point light  ...."));

    l->AttachToComponent(RootComponent , FAttachmentTransformRules::KeepRelativeTransform);
    l->RegisterComponent();  // register with render system and physics system
    AddInstanceComponent(l);  // show detail panle when selected in UnrealEditor
    l->SetRelativeTransform(FTransform(rot, rot.RotateVector(translation)));
    l->SetIntensity(intensity);
    l->SetAttenuationRadius(radius);
    l->SetSoftSourceRadius(radius);
    l->SetCastShadows(false);
    l->bUseInverseSquaredFalloff = false;
    l->LightFalloffExponent = 1;

}
