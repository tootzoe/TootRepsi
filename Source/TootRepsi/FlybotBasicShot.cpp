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



#ifdef Q_CC_MSVC
#pragma execution_character_set("UTF-8")
#endif


//------------------------------------------------------------------------





#include "FlybotBasicShot.h"
#include "TootRepsi/PrjLog.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
//
#include "PlayerBotPawn.h"

//

AFlybotBasicShot::AFlybotBasicShot()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthDelta = -1.f;
    PowerDelta = -1.f;


    SphereSceneRoot = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    SetRootComponent(SphereSceneRoot);
    SphereSceneRoot->SetSphereRadius(28.f);
    SphereSceneRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    SphereSceneRoot->OnComponentHit.AddDynamic(this, &AFlybotBasicShot::OnHit);



    FlyFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
    FlyFXComp->SetupAttachment(SphereSceneRoot  );


    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    MovementComp->InitialSpeed = 20000.f;
    MovementComp->MaxSpeed = 20000.f;
    MovementComp->ProjectileGravityScale =  0.f;
    //
    InitialLifeSpan = 2.f;

}



void AFlybotBasicShot::BeginPlay()
{
    Super::BeginPlay();
}

void AFlybotBasicShot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFlybotBasicShot::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                             UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
       // UE_LOG(LogTootRepsi, Warning, TEXT("Hit something : %s.....%hs") , *OtherActor->GetName(),  __func__);

    APlayerBotPawn* shooter = GetInstigator<APlayerBotPawn>();
    APlayerBotPawn* targetPawn = Cast<APlayerBotPawn>(OtherActor);

    if(targetPawn && targetPawn != shooter && targetPawn->GetLocalRole() == ROLE_Authority){
      targetPawn->updHealth(HealthDelta);
    }


        if(HitFXCueComp){
             UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFXCueComp, SphereSceneRoot->GetComponentLocation(),
                                                    SphereSceneRoot->GetComponentRotation());
        }


        Destroy();
}



// void AFlybotBasicShot::NotifyHit(UPrimitiveComponent *MyComp, AActor *Other, UPrimitiveComponent *OtherComp,
//                                  bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
// {
//     Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
//     UE_LOG(LogTootRepsi, Warning, TEXT("Hit something.....%hs") , __func__);


//     Destroy();
// }
