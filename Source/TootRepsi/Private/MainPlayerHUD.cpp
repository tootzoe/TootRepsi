// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerHUD.h"

#include "Components/ProgressBar.h"



void UMainPlayerHUD::updHealth(float currHealth, float maxHealth)
{
    if(HealthBar){
        HealthBar->SetPercent( currHealth / maxHealth);
    }
}

void UMainPlayerHUD::updPower(float currPower, float maxPower)
{
    if(PowerBar){
        PowerBar->SetPercent( currPower / maxPower);
    }
}
