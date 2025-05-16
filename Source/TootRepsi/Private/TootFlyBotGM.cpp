// Fill out your copyright notice in the Description page of Project Settings.


#include "TootFlyBotGM.h"


#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"

#include "PlayerBotPawn.h"
#include "PlayerBotController.h"


#include "GameplayDebugger.h"
#include "GameplayDebuggerPlayerManager.h"



ATootFlyBotGM::ATootFlyBotGM(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerControllerClass = APlayerBotController::StaticClass();

    DefaultPawnClass = APlayerBotPawn::StaticClass();

}

void ATootFlyBotGM::SetPlayerDefaults(APawn *PlayerPawn)
{
    Super::SetPlayerDefaults(PlayerPawn);


#if WITH_GAMEPLAY_DEBUGGER

   // GetWorld()->GetTimerManager().SetTimer(this, );
     GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATootFlyBotGM::tootDebugFunc1 );

    UE_LOG(LogTemp, Warning, TEXT(" WITH_GAMEPLAY_DEBUGGER has msg....%hs") , __func__);

#endif

}

#if WITH_GAMEPLAY_DEBUGGER
void ATootFlyBotGM::tootDebugFunc1()
{

    UE_LOG(LogTemp, Warning, TEXT(" Enter....%hs") , __func__);
  // UGameInstance* tmpGameInst =  GetGameInstance<UGameInstance>();

    //UKismetSystemLibrary::get

    UGameInstance* tmpGameInst = UGameplayStatics::GetGameInstance(GetWorld());
    APlayerController *pc =  tmpGameInst->GetFirstLocalPlayerController(GetWorld());



    if( pc &&  IGameplayDebugger::IsAvailable() ){

         UE_LOG(LogTemp, Warning, TEXT(" IGameplayDebugger::IsAvailable gooooo....%hs") , __func__);
        AGameplayDebuggerPlayerManager& debugManager =
                AGameplayDebuggerPlayerManager::GetCurrent(GetWorld());



        if(UInputComponent* inputComp = debugManager.GetInputComponent(*pc)){
            UE_LOG(LogTemp, Warning, TEXT(" inputComp is ready....%hs") , __func__);
            inputComp->BindKey(FKey( "x")   ,EInputEvent::IE_Pressed, this, &ATootFlyBotGM::tootDebugKeyFunc1  );

            // Removes given inputcomponent from the input stack (regardless of if it's the top, actually).
            // e.g. unbind Semicolon and Apostrophe from debug controls in PIE....
            pc->PopInputComponent(inputComp); //
        }
    }else{

        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATootFlyBotGM::tootDebugFunc1 );
    }
}

void ATootFlyBotGM::tootDebugKeyFunc1()
{
    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);

}
#endif
