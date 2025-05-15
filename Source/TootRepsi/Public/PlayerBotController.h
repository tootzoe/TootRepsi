// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerBotController.generated.h"

/**
 * 
 */
UCLASS()
class TOOTREPSI_API APlayerBotController : public APlayerController
{
	GENERATED_BODY()

    public:





    UPROPERTY()
    class UInputMappingContext* PlayerIMC;
    UPROPERTY()
    class UInputAction* MoveIA;
    UPROPERTY()
    class UInputAction* RotateIA;
    UPROPERTY()
    class UInputAction* FreeFlyIA;
    //
    UPROPERTY()
    class UInputAction* FireIA;




     // APlayerController interface
     protected:
     virtual void SetupInputComponent() override;




};
