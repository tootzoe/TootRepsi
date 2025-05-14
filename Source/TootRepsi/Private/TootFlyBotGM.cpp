// Fill out your copyright notice in the Description page of Project Settings.


#include "TootFlyBotGM.h"


#include "PlayerBotPawn.h"
#include "PlayerBotController.h"



ATootFlyBotGM::ATootFlyBotGM(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
    PlayerControllerClass = APlayerBotController::StaticClass();

    DefaultPawnClass = APlayerBotPawn::StaticClass();

}

void ATootFlyBotGM::SetPlayerDefaults(APawn *PlayerPawn)
{
    Super::SetPlayerDefaults(PlayerPawn);


}
