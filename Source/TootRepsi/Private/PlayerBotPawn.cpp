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

    ULocalPlayer* tmpLocalPlayer = tmpPC->GetLocalPlayer();
    check(tmpLocalPlayer);
    UEnhancedInputLocalPlayerSubsystem *tmpSubSys =
            tmpLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    check(tmpSubSys);

    tmpSubSys->ClearAllMappings();
    tmpSubSys->AddMappingContext(tmpPC->PlayerIMC, 0);

}

void APlayerBotPawn::botMove(const struct FInputActionValue &val)
{
    FVector inputVal = val.Get<FInputActionValue ::Axis3D>();

    AddMovementInput(GetActorRotation().RotateVector(inputVal), MoveScale);

   // UE_LOG(LogTemp, Warning, TEXT("%f - %f - %f ....%hs") , val[0],val[1],val[2], __func__);
}

void APlayerBotPawn::botRotate(const FInputActionValue &val)
{
    FRotator r(val[0] , val[1] , val[2]);

    r *= GetWorld()->GetDeltaSeconds() * RotateScale;

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

