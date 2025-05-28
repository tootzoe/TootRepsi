/**************************************************************************
**   Copyright @ 2025 TOOTzoe.com
**   Special keywords: thor 5/28/2025 2025
**   Environment variables:
**
**
**
**   E-mail : toot@tootzeo.com
**   Tel    : 13802205042
**   Website: http://www.tootzoe.com
**
**************************************************************************/


//------------------------------------------------------------------------







#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/HUD.h"
#include "GameDspHUD.generated.h"
//
/**
 *
 */
UCLASS(   )
class TOOTREPSI_API AGameDspHUD : public AHUD
{
    GENERATED_BODY()


    public:
       // AHUD interface
        virtual void DrawHUD () override;


    UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="TOOT")

    float CrosshairExpandWeight;


     private:


    void drawPlayerColorStripe(const FLinearColor& cr, float heightTop, float heightBottom);
    void drawCrosshair(const FLinearColor& cr, float maxSz , float grapSz);


};

