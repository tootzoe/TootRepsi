// Fill out your copyright notice in the Description page of Project Settings.


#include "TootMainGM.h"

#include "TootRepsiPlayerCtrlor.h"
#include "TootRepsiCharacter.h"



ATootMainGM::ATootMainGM(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{


    UE_LOG(LogTemp, Warning, TEXT("ATootMain GM........"));

    PlayerControllerClass = ATootRepsiPlayerCtrlor::StaticClass();
    DefaultPawnClass = ATootRepsiCharacter::StaticClass();



}

void ATootMainGM::SetPlayerDefaults(APawn *PlayerPawn)
{
    Super::SetPlayerDefaults(PlayerPawn);

    UE_LOG(LogTemp, Warning, TEXT("SetPlayerDefaults........"));


}



