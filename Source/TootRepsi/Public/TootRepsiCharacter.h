// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TootRepsiCharacter.generated.h"

UCLASS()
class TOOTREPSI_API ATootRepsiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATootRepsiCharacter();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PossessedBy(AController *NewController) override; 
    virtual void PostInitializeComponents() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    void setupInput(class UInputComponent* PlayerInputComponent);
    void bindInputAction(class UEnhancedInputComponent* inputComp);

    //
    void moveCharacter(const struct FInputActionValue &val);
    void lookCharacter(const struct FInputActionValue &val);
    void fireCharacter(const struct FInputActionValue &val);


 };
