// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBotPawn.h"


#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"





// Sets default values
APlayerBotPawn::APlayerBotPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnRootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
    SetRootComponent(PawnRootSphere);
    //body
    PawnBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    PawnBody->SetupAttachment(PawnRootSphere );
    //head
    PawnHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
    PawnHead->SetupAttachment(PawnBody );


    //spring arm
    PawnSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    PawnSpringArm->SetupAttachment(PawnRootSphere );
    //camera
    PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    PawnCamera->SetupAttachment(PawnSpringArm , USpringArmComponent::SocketName );



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

}

