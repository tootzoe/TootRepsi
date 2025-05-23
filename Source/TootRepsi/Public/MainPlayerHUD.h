// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOOTREPSI_API UMainPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	
    public:



	
    void updHealth(float currHealth, float maxHealth)  ;
    void updPower (float currPower, float maxPower);


    UPROPERTY(EditAnywhere,  Category="TOOT" , meta = (BindWidget))
    class UProgressBar* HealthBar;
    UPROPERTY(EditAnywhere, Category="TOOT" ,   meta = (BindWidget))
    class UProgressBar* PowerBar;


	
};
