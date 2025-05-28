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
#include "Kismet/GameplayStatics.h"
//
#include "PlayerBotPawn.h"

//

AFlybotBasicShot::AFlybotBasicShot()
{
    PrimaryActorTick.bCanEverTick = true;

    HealthDelta = -12.f;
    PowerDelta = -1.f;


    SphereSceneRoot = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
    SetRootComponent(SphereSceneRoot);
    SphereSceneRoot->SetSphereRadius(28.f);
    SphereSceneRoot->SetCollisionProfileName(TEXT("BlockAllDynamic"));

    SphereSceneRoot->OnComponentHit.AddDynamic(this, &AFlybotBasicShot::OnHit);



    FlyFXComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
    FlyFXComp->SetupAttachment(SphereSceneRoot  );
    FlyFXComp->SetAgeUpdateMode(ENiagaraAgeUpdateMode::TickDeltaTime);


    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    MovementComp->InitialSpeed = 20000.f;
    MovementComp->MaxSpeed = 20000.f;
    MovementComp->ProjectileGravityScale =  0.f;
    //
    InitialLifeSpan = 28.f;

}



void AFlybotBasicShot::BeginPlay()
{
    Super::BeginPlay();






}

void AFlybotBasicShot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

#if 0
    //test changing niagara time dilation....
    if(GetInstigator()){
      float dist = FVector::Dist(GetActorLocation(), GetInstigator()->GetActorLocation());
     // UE_LOG(LogTootRepsi, Warning, TEXT("Instigator: %s.... dist: %.3f ,  %f")  , *GetInstigator()->GetName() ,dist , DeltaTime);

      CustomTimeDilation =   1.f -  (dist / 3000.f );
    //  FlyFXComp->SetCustomTimeDilation(  FMath::Clamp (  1.f -  (dist / 3000.f  ) , .9f , 1.f) );

     // FlyFXComp->AdvanceSimulationByTime( 100.f ,  100.f);
      FlyFXComp->SetPaused(false);
      FlyFXComp->AdvanceSimulation(1, .002f);
      FlyFXComp->SetPaused(true);

     // UGameplayStatics::SetGlobalTimeDilation(GetWorld(),  1.f -  (dist / 3000.f ));

    }
#endif


}

void AFlybotBasicShot::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                             UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
       // UE_LOG(LogTootRepsi, Warning, TEXT("Hit something : %s.....%hs") , *OtherActor->GetName(),  __func__);

    APlayerBotPawn* shooter = GetInstigator<APlayerBotPawn>();
    APlayerBotPawn* targetPawn = Cast<APlayerBotPawn>(OtherActor);

    if(targetPawn && targetPawn != shooter && targetPawn->GetLocalRole() == ROLE_Authority){
        UE_LOG(LogTootRepsi, Warning, TEXT("Health delta : %f....%hs") , HealthDelta,  __func__);
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
