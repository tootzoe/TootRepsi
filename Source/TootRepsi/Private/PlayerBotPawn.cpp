// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBotPawn.h"

#include "Engine/World.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/PlayerInput.h"
#include "InputActionValue.h"
#include "InputModifiers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include  "Engine/StaticMesh.h"
#include  "PhysicsEngine/BodySetup.h"


#include "TootFlyBotGM.h"
#include "PlayerBotController.h"
#include "TootRepsi/FlybotBasicShot.h"
#include "MainPlayerHUD.h"

#include "../PrjLog.h"



// Sets default values
APlayerBotPawn::APlayerBotPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnRootDummy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnRootDummy"));
    SetRootComponent(PawnRootDummy);
    PawnRootDummy->SetVisibleFlag(false);

    // PawnRootDummy->OnComponentBeginOverlap.AddDynamic(this, &APlayerBotPawn::OnOverlapBegin1);
    // PawnRootDummy->OnComponentEndOverlap.AddDynamic(this, &APlayerBotPawn::OnOverlapEnd1);
    // PawnRootDummy->OnComponentHit. AddDynamic(this, &APlayerBotPawn::OnHit);

#if 0
    // useless , tootzoe
    TScriptDelegate<FWeakObjectPtr> onHitFunc;
    onHitFunc.BindUFunction(this, "OnHit2");

    PawnRootDummy->OnComponentBeginOverlap.Add(onHitFunc);
    PawnRootDummy->SetGenerateOverlapEvents(true);

#endif



    //body
    PawnBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    PawnBody->SetupAttachment(PawnRootDummy );
    //head
    PawnHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
    PawnHead->SetupAttachment(PawnBody );



    //spring arm
    PawnSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    PawnSpringArm->SetupAttachment(PawnRootDummy );
    PawnSpringArm->TargetArmLength = 800.f;
    //camera
    PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    PawnCamera->SetupAttachment(PawnSpringArm , USpringArmComponent::SocketName );

    // input and movement
    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
    FloatingMovement->MaxSpeed = 5000.f;
    FloatingMovement->Acceleration = 5000.f;
    FloatingMovement->Deceleration = 1000.f;


    MoveScale = 1.f;
    RotateScale = 50.f;
    bFreeFly = false;
    //
    SprintArmLenScale = 2000.f;
    SprintArmLenMax = 1000.f;
    SprintArmLenMin =0 ;
    //
     ZMovementAmplitude = 5.f;
     ZMovementFrequency = 2.f;
     ZMovementOffset = 50.f;
     //
     TiltMax = 15.f;
     TiltMoveScale = .6f;
     TiltRotateScale = .2f;
     TiltResetScale = .3f;
     MoveWithHitMax = 30;
     //
     SpeedCheckInterval = .5f ; // 0.5 second
     speedCheckLastTS = 0.f ;
     //
     MuzzleOffset = FVector(300.f ,0, 0 );
     ShotSubClass = AFlybotBasicShot::StaticClass();
     //
     bCanShoot = false;
     ShootingInterval = .2f;
     //
     MainPlayerClass = nullptr;
     MainPlayerHUD = nullptr;
     //
     MaxHealth = 100.f;
     Health = MaxHealth;
     //
     MaxPower = 25.f;
     Power = MaxPower;
     PowerRegenerateRate = 1.f;

     // We should match this to FlybotShot (life span * speed , and Squared) so they are destroyed at the same distance.   
     SetNetCullDistanceSquared(1600000000.f);
     SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

     //

}

//Called when the game starts or when spawned
void APlayerBotPawn::BeginPlay()
{
    Super::BeginPlay();


    if(IsLocallyControlled() && MainPlayerClass){
        APlayerBotController* pc = GetController<APlayerBotController>();
        check(pc);
        MainPlayerHUD = CreateWidget<UMainPlayerHUD>(pc, MainPlayerClass);
        check(MainPlayerHUD);
        MainPlayerHUD->AddToPlayerScreen();
        MainPlayerHUD->updHealth(Health , MaxHealth);
        MainPlayerHUD->updPower(Power, MaxPower);
    }


    // PawnRootDummy->OnComponentBeginOverlap.AddDynamic(this, &APlayerBotPawn::OnOverlapBegin1); // not trigged
    // PawnRootDummy->OnComponentEndOverlap.AddDynamic(this, &APlayerBotPawn::OnOverlapEnd1);  // not trigged
     PawnRootDummy->OnComponentHit. AddDynamic(this, &APlayerBotPawn::OnHit);
}

void APlayerBotPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if(MainPlayerHUD){
        MainPlayerHUD->RemoveFromParent();
        MainPlayerHUD = nullptr;
    }
}


// Called every frame
void APlayerBotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RegeneratePower();

    tryShooting();


    if(GetNetMode() == ENetMode::NM_DedicatedServer)
        return;

    if(ZMovementAmplitude)
    {
        ZMovementTotalTime += DeltaTime;
        float zMov = FMath::Sin(ZMovementTotalTime * ZMovementFrequency) * ZMovementAmplitude;
        PawnBody->SetRelativeLocation(FVector(0.f, 0.f , zMov + ZMovementOffset));

    }

    FRotator bodyRot = PawnBody->GetRelativeRotation();

    if(TiltCurrVal != 0 ){
        float tmpV = bodyRot.Roll +  TiltCurrVal;
        bodyRot.Roll = FMath::Clamp(tmpV , -TiltMax , TiltMax);
        TiltCurrVal = 0;
    }

    if(bodyRot.Roll > 0){
        float vv =  bodyRot.Roll -  TiltResetScale;
        bodyRot.Roll = FMath::Clamp(vv, 0, vv);
    }else if(bodyRot.Roll < 0 ){
        float vv =  bodyRot.Roll +  TiltResetScale;
        bodyRot.Roll = FMath::Clamp(vv, vv, 0);
    }

    PawnBody->SetRelativeRotation(bodyRot);
    PawnHead->SetRelativeRotation(FRotator(0.f, bodyRot.Roll, 0.f));

    if(Controller && Controller->GetLocalRole() == ROLE_AutonomousProxy){
        Svr_UpdPawnTransform(GetRootComponent()->GetRelativeTransform());
    }


}

// Called to bind functionality to input
void APlayerBotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* tmpEIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    APlayerBotController* tmpPC = Cast<APlayerBotController>(Controller);

    checkf(tmpEIC && tmpPC , TEXT("tmpEIC && playerController invalid...."));

    tmpEIC->BindAction(tmpPC->MoveIA, ETriggerEvent::Triggered, this, &APlayerBotPawn::botMove );
    tmpEIC->BindAction(tmpPC->RotateIA, ETriggerEvent::Triggered, this, &APlayerBotPawn::botRotate );
    tmpEIC->BindAction(tmpPC->FreeFlyIA, ETriggerEvent::Started, this, &APlayerBotPawn::botFreeFly );
    tmpEIC->BindAction(tmpPC->FireIA, ETriggerEvent::Started, this, &APlayerBotPawn::botFire );
    tmpEIC->BindAction(tmpPC->FireIA, ETriggerEvent::Completed, this, &APlayerBotPawn::botFire );
    tmpEIC->BindAction(tmpPC->SprintArmIA, ETriggerEvent::Triggered, this, &APlayerBotPawn::botSpringArmChged );


    ULocalPlayer* tmpLocalPlayer = tmpPC->GetLocalPlayer();
    check(tmpLocalPlayer);
    UEnhancedInputLocalPlayerSubsystem *tmpSubSys =
            tmpLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(tmpSubSys);

    tmpSubSys->ClearAllMappings();
    tmpSubSys->AddMappingContext(tmpPC->PlayerIMC, 0);

}

void APlayerBotPawn::OnRepHealth()
{
    if(MainPlayerHUD){
        MainPlayerHUD->updHealth(Health, MaxHealth);
    }
}

void APlayerBotPawn::OnRep_Color()
{
    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);
}



void APlayerBotPawn::Svr_UpdShooting_Implementation (bool bNewShooting)
{
    bCanShoot = bNewShooting;
}

void APlayerBotPawn::Cli_UpdPawnTransform_Implementation(FTransform Transform)
{
    GetRootComponent()->SetRelativeTransform(Transform);
}

void APlayerBotPawn::Svr_UpdPawnTransform_Implementation(FTransform Transform)
{

    float currTS = GetWorld()->GetRealTimeSeconds();
    if(speedCheckLastTS == 0){
        speedcheckLastPos = Transform.GetTranslation();
        speedCheckLastTS = currTS;
        speedCheckFrameCnt = 0;
        speedCheckPosSum = FVector::ZeroVector;
    }else{
        speedCheckPosSum += Transform.GetTranslation();
        speedCheckFrameCnt ++;

        if( currTS - speedCheckLastTS > SpeedCheckInterval){
            FVector averageVec = speedCheckPosSum / speedCheckFrameCnt;
            float tmpDist = FVector::Distance(speedcheckLastPos , averageVec);
            float tmpSpeed = tmpDist / (currTS - speedCheckLastTS); // 0.5 second speed;
           // UE_LOG(LogTootRepsi, Warning, TEXT("Client speed calculated: speed: %.3f >> frame count: %d .... ") ,tmpSpeed, speedCheckFrameCnt);

            speedCheckLastTS = currTS;
            speedCheckPosSum = FVector::ZeroVector;
            speedCheckFrameCnt = 0;

            if( tmpSpeed >   FloatingMovement->MaxSpeed * 1.08f   ){
                UE_LOG(LogTootRepsi, Warning, TEXT("!!!! Player %s moving too fast speed: %.3f  ....") , *Controller->GetName() , tmpSpeed );
                Cli_UpdPawnTransform(GetRootComponent()->GetRelativeTransform());
                return;
            }

            speedcheckLastPos = averageVec;
        }

    }


    // float distance = FVector::Dist(GetRootComponent()->GetRelativeLocation(), Transform.GetTranslation());
    // float speed = distance / GetWorld()->GetDeltaSeconds();
    // UE_LOG(LogTootRepsi, Warning, TEXT("Client speed : %s = %.3f >>>>  %.3f ....") , *Controller->GetName() , speed, GetWorld()->GetDeltaSeconds() );


    FTransform oldTsf = GetRootComponent()->GetRelativeTransform();
    FHitResult hitRsl;

    GetRootComponent()->SetRelativeTransform(Transform , true, &hitRsl);
    if(hitRsl.bBlockingHit)
    {
       // UE_LOG(LogTootRepsi, Warning, TEXT("Warning!!!! Correcting player transform : %s ....") , *Controller->GetName());
       // Cli_UpdPawnTransform(oldTsf);
        moveWithHitsCnt ++;
    }else{
        moveWithHitsCnt = 0;
    }

    if (moveWithHitsCnt > MoveWithHitMax) {
         UE_LOG(LogTootRepsi, Warning, TEXT("Correct client....%hs") , __func__);
        Cli_UpdPawnTransform(GetRootComponent()->GetRelativeTransform());
    }
}

void APlayerBotPawn::botMove(const struct FInputActionValue &val)
{
    FVector inputVal = val.Get<FInputActionValue ::Axis3D>();

    AddMovementInput(GetActorRotation().RotateVector(inputVal), MoveScale);

    TiltCurrVal += inputVal.Y * TiltMoveScale * MoveScale;

  //  UE_LOG(LogTemp, Warning, TEXT("%f - %f - %f ....%hs") , val[0],val[1],val[2], __func__);

}

void APlayerBotPawn::botRotate(const FInputActionValue &val)
{
    FRotator r(val[0] , val[1] , val[2]);

    r *= GetWorld()->GetDeltaSeconds() * RotateScale;

    TiltCurrVal += r.Yaw * TiltRotateScale;



    if(bFreeFly) {
        AddActorLocalRotation(r);
    }else{
        r += GetActorRotation();
        r.Pitch = FMath::ClampAngle(r.Pitch, -89.9f, 89.9f);
        r.Roll = 0;

        SetActorRotation(r);
    }
}

void APlayerBotPawn::botFreeFly( )
{
    bFreeFly= !bFreeFly;

#if 0
    // get UStaticMesh from UStaticMeshComponent,
    // get Elements from UStaticMesh : mesh->GetBodySetup()->AggGeom.GetElement(0);

    int32 elemCnt = PawnRootDummy->GetStaticMesh()->GetBodySetup()->AggGeom.GetElementCount();

     for(int32 i = 0 ; i < elemCnt ; i ++){

         FKShapeElem const * elem = PawnRootDummy->GetStaticMesh()->GetBodySetup()->AggGeom.GetElement(i);

         FVector vec =   elem->GetTransform().GetLocation();

         UE_LOG(LogTemp, Warning, TEXT("Vec.x = %.3f , y= %.3f , z= %.3f ") ,vec.X , vec.Y, vec.Z);

     }
#endif

}

void APlayerBotPawn::botFire(const FInputActionValue& val)
{
  // UE_LOG(LogTemp, Warning, TEXT("val:  %d  ...%hs")   , val[0] > 0   ,   __func__);

   bCanShoot = val[0] > 0;

   Svr_UpdShooting(bCanShoot);

}

void APlayerBotPawn::botSpringArmChged(const FInputActionValue &val)
{
   // float v = val.Get<FInputActionValue::Axis1D>();

    float tmpVal = GetWorld()->GetDeltaSeconds() * SprintArmLenScale * val[0];
    tmpVal += PawnSpringArm->TargetArmLength;
    PawnSpringArm->TargetArmLength  = FMath::Clamp(tmpVal , SprintArmLenMin, SprintArmLenMax);
}

void APlayerBotPawn::RegeneratePower()
{
    Power = FMath::Clamp(Power + PowerRegenerateRate * GetWorld()->DeltaTimeSeconds , 0.f , MaxPower);

    if(MainPlayerHUD){
        MainPlayerHUD->updPower(Power, MaxPower);
    }

}

void APlayerBotPawn::tryShooting()
{
    float currTS  = GetWorld()->GetRealTimeSeconds();

    float powerDelta = Cast<AFlybotBasicShot>(ShotSubClass->GetDefaultObject())->PowerDelta;

    if(!bCanShoot || currTS - shootingLastTS < ShootingInterval || Power + powerDelta <= 0){
        return;
    }



        // UE_LOG(LogTootRepsi, Warning, TEXT("Shot Spawned %s , %.3f , %s , %s") ,
        //        *GetName(), currTS, IsNetMode(NM_Client) ? TEXT("Client") : TEXT("Server") , Controller ? TEXT("Controlled") : TEXT("Simulated") );

        FTransform t = PawnBody->GetComponentTransform();
        FRotator r = t.Rotator();
        FVector v = t.GetTranslation() + r.RotateVector(MuzzleOffset);
        AFlybotBasicShot* shot =   GetWorld()->SpawnActor<AFlybotBasicShot>(ShotSubClass, v, r);

        if(shot){
            shootingLastTS = currTS;
            shot->SetInstigator( this);
            Power += powerDelta;
            if(MainPlayerHUD){
                MainPlayerHUD->updPower(Power, MaxPower);
            }
        }


}



void APlayerBotPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(APlayerBotPawn,  bCanShoot , COND_SimulatedOnly);
    DOREPLIFETIME_CONDITION(APlayerBotPawn,  Health , COND_OwnerOnly);
}

void APlayerBotPawn::updHealth(float healthDelta)
{
    Health = FMath::Clamp(Health + healthDelta, 0.f , MaxHealth);
    if(MainPlayerHUD){
        MainPlayerHUD->updHealth(Health, MaxHealth);
    }

    if(Health == 0.f){
        UE_LOG(LogTootRepsi, Warning, TEXT("Bot dead....%hs") , __func__);
    }

   // CustomTimeDilation

}

void APlayerBotPawn::OnOverlapBegin1(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);
}

void APlayerBotPawn::OnOverlapEnd1(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);
}




void APlayerBotPawn::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                           UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit)
{
     // UE_LOG(LogTemp, Warning, TEXT("msg....hit comp: %s  , other actor: %s , %s  ,  idx: %d") ,
     //        *HitComponent->GetName() , *OtherActor->GetName(), *OtherComponent->GetName(),
     //        Hit.ElementIndex
     //        );


    int32 elemCnt = PawnRootDummy->GetStaticMesh()->GetBodySetup()->AggGeom.GetElementCount();

    float nearestDist = UE_FLOAT_HUGE_DISTANCE;
    FKShapeElem   * tmpElem = nullptr;

     for(int32 i = 0 ; i < elemCnt ; i ++){
          FKShapeElem   * const elem = PawnRootDummy->GetStaticMesh()->GetBodySetup()->AggGeom.GetElement(i);

         float tmpDist = FVector::Distance( elem->GetTransform().GetLocation() + PawnRootDummy->GetComponentLocation() , Hit.ImpactPoint);

         if(tmpDist < nearestDist){
             nearestDist = tmpDist;
             tmpElem = elem;
         }
     }

     if(tmpElem){
          UE_LOG(LogTemp, Warning, TEXT(" collision obj is : %s") , *tmpElem->GetName().ToString()  );
     }
}

void APlayerBotPawn::auth_setPlayerColor(const FLinearColor &cr)
{
    checkf(HasAuthority() , TEXT("Error: auth_setPlayerColor() called on client !!...."));
    currColor = cr;

    OnRep_Color();
}





