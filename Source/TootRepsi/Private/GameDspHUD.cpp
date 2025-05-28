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


#include "GameDspHUD.h"


#include "Engine/Canvas.h"

#include "PlayerBotPawn.h"






void AGameDspHUD::DrawHUD()
{
    Super::DrawHUD();

    APlayerBotPawn * p = Cast<APlayerBotPawn>(GetOwningPawn());


    if(p && Canvas){

       drawPlayerColorStripe( p->currColor, 4.f , 8.f);


       // two testing numbers
       static float testFlag = 1.f;
       static int32  testNum = 0;
       if(testNum ++ % 60 == 0){
           if(testFlag == 0) testFlag = 1.f;
           else testFlag = 0;
       }

       CrosshairExpandWeight = FMath::FInterpTo(CrosshairExpandWeight, testFlag, RenderDelta, 8.f);

       float crosshairSZ  = FMath::Lerp(16.0 , 28.0 , CrosshairExpandWeight);
       float gapSZ  = FMath::Lerp(8.0 , 16.0 , CrosshairExpandWeight);

       drawCrosshair(p->currColor , crosshairSZ  , gapSZ );




    }

   // UE_LOG(LogTemp, Warning, TEXT("msg....%hs  , CrosshairExpandWeight = %f") , __func__ , CrosshairExpandWeight);


   // drawCrosshair();


}

void AGameDspHUD::drawPlayerColorStripe(const FLinearColor &cr, float heightTop, float heightBottom)
{
    check(Canvas);

    DrawRect(cr, .0f, .0f , Canvas->SizeX , heightTop);
    DrawRect(cr, .0f, Canvas->SizeY - heightBottom , Canvas->SizeX , heightBottom);

}

void AGameDspHUD::drawCrosshair(const FLinearColor &cr, float maxSz, float grapSz)
{
     check(Canvas);

    float oriX = Canvas->SizeX / 2.f;
    float oriY = Canvas->SizeY / 2.f;

    float aimOffset = maxSz / 2.f;
    float gapOffset = grapSz / 2.f;
    float thickness = 3.f;

    DrawLine(oriX - aimOffset , oriY, oriX - gapOffset , oriY, cr, thickness);
    DrawLine(oriX + gapOffset , oriY, oriX + aimOffset , oriY, cr, thickness);
    DrawLine( oriX, oriY - aimOffset , oriX, oriY - gapOffset ,  cr, thickness);
    DrawLine( oriX, oriY + gapOffset , oriX, oriY + aimOffset ,  cr, thickness);


}
