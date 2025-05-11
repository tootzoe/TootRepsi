// Fill out your copyright notice in the Description page of Project Settings.


#include "TootRepsiCharacter.h"

#include <UObject/ConstructorHelpers.h>
#include <Engine/LocalPlayer.h>
#include <Kismet/GameplayStatics.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <InputMappingContext.h>

#include "TootRepsi/PrjLog.h"


// Sets default values
ATootRepsiCharacter::ATootRepsiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationPitch = true;

    //RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));



    UE_LOG(LogTootRepsi, Warning, TEXT("msg....%hs") , __func__);

}

// Called when the game starts or when spawned
void ATootRepsiCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATootRepsiCharacter::setupInput(UInputComponent *PlayerInputComponent)
{
    APlayerController* tmpController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    checkf(tmpController , TEXT("tmpController invalid...."));
    checkf(PlayerInputComponent , TEXT("PlayerInputComponent invalid...."));

    APlayerController* tmpPC = Cast<APlayerController>(tmpController);

    checkf(tmpPC , TEXT("Player controller invalid...."));

    UEnhancedInputLocalPlayerSubsystem* tmpPlayerSubsys =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(tmpPC->GetLocalPlayer());


    checkf(tmpPlayerSubsys , TEXT("Local player subsystem invalid...."));

     // we test loadObj first....
     UInputMappingContext* tmpIMC = LoadObject<UInputMappingContext>(NULL, TEXT("/Game/Input/IMC_TootPlayer.IMC_TootPlayer"));

     checkf(tmpIMC , TEXT("tmpIMC invalid...."));

     UEnhancedInputComponent* tmpEIComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

      tmpPlayerSubsys->AddMappingContext(tmpIMC, 0);


      bindInputAction(tmpEIComp);



}

void ATootRepsiCharacter::bindInputAction(UEnhancedInputComponent *inputComp)
{

   // UE_LOG(LogTemp, Warning, TEXT("%hs.......") , __func__);

    // test using ConstructorHelpers
   // static ConstructorHelpers::FObjectFinder<UInputAction> moveAct(TEXT("InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));

    UInputAction* moveAct =  LoadObject<UInputAction>(NULL, TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
    UInputAction* lookAct =  LoadObject<UInputAction>(NULL, TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
    UInputAction* fireAct =  LoadObject<UInputAction>(NULL, TEXT("/Game/Input/Actions/IA_Fire.IA_Fire"));


    if(!moveAct || !lookAct || !fireAct){
        UE_LOG(LogTemp, Warning, TEXT("Bind action failed......"));
        return;
    }


    inputComp->BindAction(moveAct , ETriggerEvent::Triggered , this, &ATootRepsiCharacter::moveCharacter);
    inputComp->BindAction(lookAct , ETriggerEvent::Triggered , this, &ATootRepsiCharacter::lookCharacter);
    inputComp->BindAction(fireAct , ETriggerEvent::Started , this, &ATootRepsiCharacter::fireCharacter);


}

void ATootRepsiCharacter::moveCharacter(const  FInputActionValue& val)
{
    FVector2D xyVal = val.Get<FVector2D>();
     AddMovementInput(GetActorForwardVector() , xyVal.Y);
     AddMovementInput(GetActorRightVector() , xyVal.X);
}

void ATootRepsiCharacter::lookCharacter(const FInputActionValue &val)
{
    FVector2D xyVal = val.Get<FVector2D>();
     AddControllerPitchInput(-xyVal.Y);
     AddControllerYawInput(xyVal.X);

}

void ATootRepsiCharacter::fireCharacter(const FInputActionValue &val)
{

    UE_LOG(LogTemp, Warning, TEXT("fire....") );
}

// Called every frame
void ATootRepsiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATootRepsiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    setupInput(PlayerInputComponent);

}



void ATootRepsiCharacter::PossessedBy(AController *NewController)
{
    UE_LOG(LogTemp, Warning, TEXT("Charater Possessed....."));

}


void ATootRepsiCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();


    UE_LOG(LogTemp, Warning, TEXT("%hs") , __func__);

}
