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

    // APlayerController interface
    public:
    virtual bool InputTouch(const FInputDeviceId DeviceId, uint32 Handle, ETouchType::Type Type, const FVector2D &TouchLocation, float Force, uint32 TouchpadIndex, const uint64 Timestamp) override;




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
    UPROPERTY()
    class UInputAction* SprintArmIA;




     // APlayerController interface
     protected:
     virtual void SetupInputComponent() override;





 };
