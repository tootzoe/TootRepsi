// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TootFlyBotGM.generated.h"

/**
 * 
 */
UCLASS()
class TOOTREPSI_API ATootFlyBotGM : public AGameModeBase
{
	GENERATED_BODY()
	
  public:
  explicit ATootFlyBotGM(const FObjectInitializer& ObjectInitializer) ;

  virtual void SetPlayerDefaults(APawn *PlayerPawn) override;


    private:

#if WITH_GAMEPLAY_DEBUGGER
   // UFUNCTION()
    void tootDebugFunc1();
    void tootDebugKeyFunc1();
#endif

};
