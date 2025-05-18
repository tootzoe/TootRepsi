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


#include "PlayerBotController.h"

#include "../PrjLog.h"



// Sets default values
APlayerBotPawn::APlayerBotPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnRootDummy = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnRootDummy"));
    SetRootComponent(PawnRootDummy);
    PawnRootDummy->SetVisibleFlag(false);
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

     SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

// Called when the game starts or when spawned
// void APlayerBotPawn::BeginPlay()
// {
// 	Super::BeginPlay();
	
// }

// Called every frame
void APlayerBotPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


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
    tmpEIC->BindAction(tmpPC->SprintArmIA, ETriggerEvent::Triggered, this, &APlayerBotPawn::botSpringArmChged );


    ULocalPlayer* tmpLocalPlayer = tmpPC->GetLocalPlayer();
    check(tmpLocalPlayer);
    UEnhancedInputLocalPlayerSubsystem *tmpSubSys =
            tmpLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(tmpSubSys);

    tmpSubSys->ClearAllMappings();
    tmpSubSys->AddMappingContext(tmpPC->PlayerIMC, 0);

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
            UE_LOG(LogTootRepsi, Warning, TEXT("Client speed calculated: speed: %.3f >> frame count: %d .... ") ,tmpSpeed, speedCheckFrameCnt);

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
}

void APlayerBotPawn::botFire()
{
   UE_LOG(LogTemp, Warning, TEXT(" ...%hs")   ,   __func__);
}

void APlayerBotPawn::botSpringArmChged(const FInputActionValue &val)
{
   // float v = val.Get<FInputActionValue::Axis1D>();

    float tmpVal = GetWorld()->GetDeltaSeconds() * SprintArmLenScale * val[0];
    tmpVal += PawnSpringArm->TargetArmLength;
    PawnSpringArm->TargetArmLength  = FMath::Clamp(tmpVal , SprintArmLenMin, SprintArmLenMax);
}

