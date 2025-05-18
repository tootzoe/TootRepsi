// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "MapRoom.generated.h"


UCLASS()
class TOOTREPSI_API AMapRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapRoom();

protected:
    // Called when the game starts or when spawned
    //virtual void BeginPlay() override;

// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;

	
	

// AActor interface
 public:
    virtual void OnConstruction(const FTransform &Transform) override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

    UPROPERTY(EditAnywhere, Category="TOOT")
    class UInstancedStaticMeshComponent* Walls;
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UInstancedStaticMeshComponent* Edges;
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UInstancedStaticMeshComponent* Corners;    
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UInstancedStaticMeshComponent* TubeWalls;
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UInstancedStaticMeshComponent* Tubes;
    //
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom ))
    uint32 PositiveXTubeCount;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom))
    uint32 NegativeXTubeCount;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom))
    uint32 PositiveYTubeCount;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom))
    uint32 NegativeYTubeCount;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom))
    uint32 PositiveZTubeCount;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMax = 1000, RebuildMapRoom))
    uint32 NegativeZTubeCount;




    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 0 , RebuildMapRoom ))
    float GridSize;

    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 1 , ClampMax = 20 , RebuildMapRoom))
    uint32 RoomSize;

    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 0  , RebuildMapRoom))
    float WallThickness;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (  RebuildMapRoom))
    float EdgeCollisionOffset;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 3  ,ClampMax = 24 , RebuildMapRoom))
    uint32 TubeCollisionSides;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 0   , RebuildMapRoom))
    float TubeCollisionRadius;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 0   , RebuildMapRoom))
    float TubeCollisionThickness;
    UPROPERTY(EditAnywhere, Category="TOOT" , meta = (ClampMin = 0   , ClampMax = 1 ,RebuildMapRoom))
    float TubeCollisionYScale;


    private:

    void placePointLight(float intensity , float radius ,const FRotator& rot ,const FVector& translation);

    void placeTubeMeshInstance(uint32 tubeCount , const FRotator& rot);
    void placeTubeCollisionBox(const FVector& extent , const FRotator& rot , const FVector& tran , const FRotator& sideRot = FRotator::ZeroRotator);
    template<class T> T* placeComp(  const FTransform& transform);



    int32 wallOffset;
    float lastGridSz;
    uint32 lastRoomSz;

    TArray<class UPointLightComponent* > pointArr ;




    bool doRebuild;


    };
