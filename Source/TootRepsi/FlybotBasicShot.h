/**************************************************************************
**   Copyright @ 2025 TOOTzoe.com
**   Special keywords: thor 5/23/2025 2025
**   Environment variables:
**
**
**
**   E-mail : toot@tootzeo.com
**   Tel    : 13802205042
**   Website: http://www.tootzoe.com
**
**************************************************************************/





#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlybotBasicShot.generated.h"
//
/**
 *
 */
UCLASS()
class TOOTREPSI_API AFlybotBasicShot : public AActor
{
    GENERATED_BODY()

   public:
   AFlybotBasicShot();

    virtual void Tick(float DeltaTime) override;

    //virtual void NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit) override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
               FVector NormalImpulse, const FHitResult& Hit);



    UPROPERTY(EditDefaultsOnly, Category="TOOT")
    class USphereComponent* SphereSceneRoot;

    UPROPERTY(EditAnywhere, Category="TOOT")
    class UNiagaraComponent* FlyFXComp;


    UPROPERTY(EditAnywhere, Category="TOOT")
    class UProjectileMovementComponent* MovementComp;

    UPROPERTY(EditAnywhere, Category="TOOT")
    class UNiagaraSystem* HitFXCueComp;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float HealthDelta;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float PowerDelta;

    protected:
    virtual void BeginPlay() override;

    private:


  };


