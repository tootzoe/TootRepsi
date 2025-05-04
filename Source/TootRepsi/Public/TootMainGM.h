// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TootMainGM.generated.h"

/**
 * 
 */
UCLASS()
class TOOTREPSI_API ATootMainGM : public AGameModeBase
{
	GENERATED_BODY()
	
   public:
   ATootMainGM(const FObjectInitializer& ObjectInitializer);
   // AGameModeBase interface
   virtual void SetPlayerDefaults(APawn *PlayerPawn) override;





};
