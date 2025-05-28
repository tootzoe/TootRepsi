// Fill out your copyright notice in the Description page of Project Settings.


#include "TootFlyBotGM.h"


#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "EngineUtils.h"


#include "PlayerBotPawn.h"
#include "PlayerBotController.h"


#include "GameplayDebugger.h"
#include "GameplayDebuggerPlayerManager.h"
#include "GameDspHUD.h"


ATootFlyBotGM::ATootFlyBotGM(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerControllerClass = APlayerBotController::StaticClass();

    DefaultPawnClass = APlayerBotPawn::StaticClass();

    HUDClass = AGameDspHUD::StaticClass();


    PlayerColors.Add(FColor::Red);
    PlayerColors.Add(FColor::Green);
    PlayerColors.Add(FColor::Blue);
    PlayerColors.Add(FColor::Yellow);
    PlayerColors.Add(FColor::Cyan);


    lastPlayerCrIdx = -1;

}

void ATootFlyBotGM::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    UE_LOG(LogTemp, Warning, TEXT("MapName: %s....%hs") , *MapName , __func__);


    for (TActorIterator<APlayerStart> it(GetWorld()) ; it ; ++it) {
        // #include "EngineUtils.h"  ....  TArray.Add(*it);
        playerStartsArr.Add(*it);
        UE_LOG(LogTemp, Warning, TEXT("Found player start: %s ....%hs") , *(*it)->GetName() , __func__);
    }



}


void ATootFlyBotGM::SetPlayerDefaults(APawn *PlayerPawn)
{
    Super::SetPlayerDefaults(PlayerPawn);

    APlayerBotPawn* newPawn = Cast<APlayerBotPawn>(PlayerPawn);
    if(newPawn){
        int32 crIdx = (lastPlayerCrIdx + 1) % PlayerColors.Num();

        if(PlayerColors.IsValidIndex(crIdx)){
            newPawn->auth_setPlayerColor(PlayerColors[crIdx]);

            lastPlayerCrIdx = crIdx;
        }

    }


//  for testing adding debug key
#if WITH_GAMEPLAY_DEBUGGER

   // GetWorld()->GetTimerManager().SetTimer(this, );
    // GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATootFlyBotGM::tootDebugFunc1 );

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

        //
       // GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATootFlyBotGM::tootDebugFunc1 );
    }
}

void ATootFlyBotGM::tootDebugKeyFunc1()
{
    UE_LOG(LogTemp, Warning, TEXT("msg....%hs") , __func__);

}
#endif


void ATootFlyBotGM::PostLogin(APlayerController *NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

FString ATootFlyBotGM::InitNewPlayer(APlayerController *NewPlayerController, const FUniqueNetIdRepl &UniqueId, const FString &Options, const FString &Portal)
{
    if(playerStartsArr.Num() < 1){
        UE_LOG(LogTemp, Warning, TEXT("No more player allowed....%hs") , __func__);
        return FString(TEXT("No player start available...."));
    }

    NewPlayerController->StartSpot = playerStartsArr.Pop();

    UE_LOG(LogTemp, Warning, TEXT("Born Player %s at %s ....%hs") ,
           *NewPlayerController->GetName() , *NewPlayerController->StartSpot->GetName() , __func__);

   return Super::InitNewPlayer(NewPlayerController,UniqueId, Options, Portal);



}

void ATootFlyBotGM::PreLogin(const FString &Options, const FString &Address, const FUniqueNetIdRepl &UniqueId, FString &ErrorMessage)
{
    if(playerStartsArr.Num() < 1){
        ErrorMessage = TEXT("Server full already.....");
     }

    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}
