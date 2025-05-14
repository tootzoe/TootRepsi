// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBotPawn.generated.h"

UCLASS()
class TOOTREPSI_API APlayerBotPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBotPawn();



    UPROPERTY(EditAnywhere, Category="TOOT")
    class USphereComponent* PawnRootSphere;
    UPROPERTY(EditAnywhere, Category="TOOT")
    UStaticMeshComponent* PawnBody;
    UPROPERTY(EditAnywhere, Category="TOOT")
    UStaticMeshComponent* PawnHead;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    class USpringArmComponent* PawnSpringArm;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UCameraComponent* PawnCamera;





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
  //  virtual void BeginPlay() override;

	
	
};
