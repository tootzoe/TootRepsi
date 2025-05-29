// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBotController.h"



#include "InputMappingContext.h"
#include "InputCoreTypes.h"
#include "InputModifiers.h"
#include "GameFramework/TouchInterface.h"
#include "Kismet/GameplayStatics.h"

#include "GenericPlatform/GenericPlatformProcess.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#include "TootRepsi/TootDefs.h"
#include "PlayerBotPawn.h"



static void mapInputKey(UInputMappingContext* imc, UInputAction* ia ,
                        FKey k, bool isNegate = false, bool isSwizzle = false,
                        EInputAxisSwizzle swOrder = EInputAxisSwizzle::YXZ);
 /////////////
/////////////
void APlayerBotController::SetupInputComponent()
{
    Super::SetupInputComponent();


    UE_LOG(LogTemp, Warning, TEXT("APlayerBotController....%hs") , __func__);

//#if TOOT_MOBILE

    if(PawnTouchInterface){
         ActivateTouchInterface(PawnTouchInterface);
         UE_LOG(LogTemp, Warning, TEXT("PawnTouchInterface enabled....%hs") , __func__);
    }else{
        UE_LOG(LogTemp, Warning, TEXT("PawnTouchInterface nullptr....%hs") , __func__);
    }

//#endif




    PlayerIMC = NewObject<UInputMappingContext>(this );


    MoveIA = NewObject<UInputAction>(this );
    MoveIA->ValueType = EInputActionValueType::Axis3D;
    RotateIA = NewObject<UInputAction>(this );
    RotateIA->ValueType = EInputActionValueType::Axis3D;
    FreeFlyIA = NewObject<UInputAction>(this );
    FireIA = NewObject<UInputAction>(this );
    SprintArmIA = NewObject<UInputAction>(this );
    SprintArmIA->ValueType = EInputActionValueType::Axis1D;

    mapInputKey(PlayerIMC, MoveIA, EKeys::W);
    mapInputKey(PlayerIMC, MoveIA, EKeys::S ,true);
    mapInputKey(PlayerIMC, MoveIA, EKeys::D ,false, true);
    mapInputKey(PlayerIMC, MoveIA, EKeys::A ,true, true);
    mapInputKey(PlayerIMC, MoveIA, EKeys::SpaceBar ,false, true , EInputAxisSwizzle::ZYX);
    mapInputKey(PlayerIMC, MoveIA, EKeys::LeftShift ,true, true , EInputAxisSwizzle::ZYX);
    //
    mapInputKey(PlayerIMC, RotateIA, EKeys::MouseY);
    mapInputKey(PlayerIMC, RotateIA, EKeys::MouseX , false,true);
    mapInputKey(PlayerIMC, RotateIA, EKeys::E ,false, true , EInputAxisSwizzle::ZYX);
    mapInputKey(PlayerIMC, RotateIA, EKeys::Q ,true, true, EInputAxisSwizzle::ZYX);
    //
    mapInputKey(PlayerIMC, FreeFlyIA, EKeys::F);
    mapInputKey(PlayerIMC, SprintArmIA, EKeys::MouseWheelAxis);


    //
    mapInputKey(PlayerIMC, FireIA, EKeys::LeftMouseButton  );


}





///////////////////////////////////////  static functions    /////////////////////////////////////////////////////////
///////////////////////////////////////  static functions    /////////////////////////////////////////////////////////
static  void mapInputKey(UInputMappingContext* imc, UInputAction* ia ,
                        FKey k, bool isNegate , bool isSwizzle ,
                        EInputAxisSwizzle swOrder )
{
    UObject* p = imc->GetOuter();
    FEnhancedActionKeyMapping& m = imc->MapKey(ia, k);

    if(isNegate){
        UInputModifierNegate* n = NewObject<UInputModifierNegate>(p );
        m.Modifiers.Add(n);
    }

    if(isSwizzle)
    {
        UInputModifierSwizzleAxis* s = NewObject<UInputModifierSwizzleAxis>(p );
        s->Order = swOrder;
        m.Modifiers.Add(s);
    }

}


bool APlayerBotController::InputTouch(const FInputDeviceId DeviceId, uint32 Handle, ETouchType::Type Type,
                                      const FVector2D &TouchLocation, float Force, uint32 TouchpadIndex,
                                      const uint64 Timestamp)
{


    return Super::InputTouch(DeviceId, Handle, Type , TouchLocation, Force , TouchpadIndex , Timestamp);
}


void APlayerBotController::BeginPlay()
{
    Super::BeginPlay();


   // const APlayerBotPawn* pa =   GetPawn<APlayerBotPawn>();
 #if 0
     UTouchInterface* const ti = LoadObject<UTouchInterface>(this, TEXT("/Engine/MobileResources/HUD/DefaultVirtualJoysticks.DefaultVirtualJoysticks"));


     if(IsValid(ti)){
       // ActivateTouchInterface(pa->PawnTouchInterface);
        ActivateTouchInterface(ti);
    }
#endif
    // if(IsValid(pa) && pa->PawnTouchInterface){
    //     UE_LOG(LogTemp, Warning, TEXT("pa okkkkkkkkkkkkk....%hs") , __func__);

    //     ActivateTouchInterface(pa->PawnTouchInterface);

    // }
}
