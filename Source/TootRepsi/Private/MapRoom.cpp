// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"

#include "UObject/UnrealType.h"





#include "TootRepsi/PrjLog.h"






// Sets default values
AMapRoom::AMapRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    GridSize = 1000.f;
    RoomSize = 3;
    WallThickness = 50.f;
    EdgeCollisionOffset = -250;
    TubeCollisionSides = 24;
    TubeCollisionRadius = 200.f;
    TubeCollisionThickness = 80.f;
    TubeCollisionYScale = .9f;

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
//     Super::BeginPlay();

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
    //
    TArray<UBoxComponent*> tubeBoxCollisionArr;
    GetComponents<UBoxComponent>(tubeBoxCollisionArr);
    for(auto b : tubeBoxCollisionArr)
        b->DestroyComponent();


#endif



    int32 halfSz = RoomSize / 2;
     wallOffset = (halfSz + 1) * GridSize;
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
    for ( FRotator  r : tmpRotatorArr) {
        //UE_LOG(LogTemp, Warning, TEXT("Idx %d ....%hs") , tmpRotatorArr.IndexOfByKey(r), __func__);
        placeTubeMeshInstance(tmpTubeCntPerSideArr[tmpRotatorArr.IndexOfByKey(r)], r);
    }

    // place edge collision boxes
    tmpWallPos.X = (wallOffset + EdgeCollisionOffset);
    tmpWallPos.Y = 0;
    tmpWallPos.Z = tmpWallPos.X;
    FVector edgeExtBox(WallThickness / 2.f , wallOffset, GridSize / 2.f);
    const TArray<FRotator> heightRotArr  = {FRotator(-90.f,0.f,0.f) , FRotator(45.f,45.f,0.f), FRotator(180.f,0.f,0.f) };
    const TArray<FRotator> selfRotArr    = {FRotator(45.f,0.f,0.f) , FRotator(45.f,0.f, 90.f), FRotator(45.f,0.f,0.f) };
    for (auto r : tmpEdgesRotatorArr) {
        for (int32 i = 0; i < heightRotArr.Num(); ++i) {
            placeTubeCollisionBox(edgeExtBox, r + heightRotArr[i], tmpWallPos , selfRotArr[i]);
        }
    }


    // place light
    tmpWallPos = FVector::ZeroVector;
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
    auto l = placeComp<UPointLightComponent>(FTransform(rot, rot.RotateVector(translation)));

    l->SetIntensity(intensity);
    l->SetAttenuationRadius(radius);
    l->SetSoftSourceRadius(radius);
    l->SetCastShadows(false);
    l->bUseInverseSquaredFalloff = false;
    l->LightFalloffExponent = 1;
}

void AMapRoom::placeTubeMeshInstance(uint32 tubeCount, const FRotator &rot)
{
    FVector tran(-wallOffset, 0.f , 0.f);
    FVector extBox (WallThickness / 2.f ,wallOffset ,wallOffset  );

   // DrawDebugBox(GetWorld(), tran, extBox , FColor::Yellow , false, 999.f ,0 , 4.f );

    if(tubeCount < 1)
    {
        placeTubeCollisionBox(extBox, rot, tran);
        return;
    }

     tran.Y = (wallOffset / 2.f) - (GridSize / 4.f);
     tran.Z = (wallOffset / 2.f) + (GridSize / 4.f);

    extBox.Y = tran.Z;
    extBox.Z = tran.Y;



    //DrawDebugLine(GetWorld(), tran,  extBox, FColor::Green, true, -1.f, 0, 2.f);
   // DrawDebugBox(GetWorld(), tran, extBox , FColor::Red , false, 999.f ,0 , 4.f );

    const TArray<float> tmpArr1 = {0, 90.f, 180.f , 270.f};

    for(auto f : tmpArr1){
        placeTubeCollisionBox(extBox,  rot + FRotator(0.f ,0.f , f), tran);
    }

#define TUBE_WALL_EXTRA_POSITIVE_OFFSET 525

   // tran.X = 0;
    tran.Y = 0;
    tran.Z = 0;

    placePointLight(1.f , GridSize , rot, tran);

    for (uint32 i = 1; i < tubeCount; ++i) {
       // tran.X = i * GridSize + wallOffset;
        tran.X = - (wallOffset + TUBE_WALL_EXTRA_POSITIVE_OFFSET + GridSize * i);
        addMeshInstance(Tubes, rot , rot , tran);
        placePointLight(1.f , GridSize, rot, tran);
    }

    extBox.X  = tubeCount * GridSize / 2.f - (GridSize - WallThickness) / 4.f;
    extBox.Y = GridSize / 2.f * TubeCollisionYScale;
    extBox.Z = TubeCollisionThickness / 2.f  ;

    tran.X = - (wallOffset   - (WallThickness / 2.f ) + extBox.X);



    for (uint32 i = 0; i < TubeCollisionSides; ++i) {
        float a = (float(i) / float( TubeCollisionSides ) ) * (2.f * PI);
         tran.Y = FMath::Sin(a) * TubeCollisionRadius;
         tran.Z = FMath::Cos(a) * TubeCollisionRadius;

        // FMath::SinCos(&tran.Y , &tran.Z , a );
        // tran.Y  *= TubeCollisionRadius;
        // tran.Z *=  TubeCollisionRadius;

        placeTubeCollisionBox(extBox, rot, tran, FRotator(0.f, 0.f , a * (180.f / PI)));

    }

}


template<class T>
 T* AMapRoom::placeComp( const FTransform& transform)
{

    T* c = NewObject<T>(this, MakeUniqueObjectName(this, T::StaticClass()));
    c->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    c->RegisterComponent();
    AddInstanceComponent(c);
    c->SetRelativeTransform(transform );

    return c;
}




void AMapRoom::placeTubeCollisionBox(const FVector &extent, const FRotator &rot, const FVector &tran, const FRotator &sideRot)
{
    UBoxComponent* bc = placeComp<UBoxComponent>( FTransform( rot + sideRot, rot.RotateVector(tran))  );
    bc->SetCollisionProfileName(TEXT("BlockAll"));
    bc->SetBoxExtent(extent);
}

