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

    virtual void InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage) override;
   virtual void SetPlayerDefaults(APawn *PlayerPawn) override;
    virtual void PreLogin(const FString &Options, const FString &Address, const FUniqueNetIdRepl &UniqueId, FString &ErrorMessage) override;

    virtual void PostLogin(APlayerController *NewPlayer) override;

    protected:
    virtual FString InitNewPlayer(APlayerController *NewPlayerController, const FUniqueNetIdRepl &UniqueId, const FString &Options, const FString &Portal) override;


    public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere,  Category="TOOT")
    TArray<FLinearColor> PlayerColors;





    private:


#if WITH_GAMEPLAY_DEBUGGER
   // UFUNCTION()
    void tootDebugFunc1();
    void tootDebugKeyFunc1();
#endif




    private:

    TArray<class APlayerStart*> playerStartsArr;

    int32 lastPlayerCrIdx;


 };
