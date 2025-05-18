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
    UStaticMeshComponent* PawnRootDummy;
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




    // input action
    UPROPERTY(EditAnywhere, Category="TOOT")
    class UFloatingPawnMovement* FloatingMovement;
    // input action
    UPROPERTY(EditAnywhere, Category="TOOT")
    float MoveScale;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float RotateScale;
    UPROPERTY(EditAnywhere, Category="TOOT")
    bool bFreeFly;

    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float SprintArmLenScale;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float SprintArmLenMin;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float SprintArmLenMax;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float ZMovementAmplitude;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float ZMovementFrequency;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float ZMovementOffset;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float TiltMoveScale;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float TiltRotateScale;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float TiltMax;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float TiltResetScale;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    uint32 MoveWithHitMax;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float SpeedCheckInterval;






public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    UFUNCTION(Server, Unreliable, Category = "TOOT")
    void Svr_UpdPawnTransform(FTransform Transform);
    UFUNCTION(Client, Unreliable, Category = "TOOT")
    void Cli_UpdPawnTransform(FTransform Transform);

protected:
    // Called when the game starts or when spawned
  //  virtual void BeginPlay() override;

    private:


    void botMove(const struct FInputActionValue& val);
    void botRotate(const struct FInputActionValue& val);
    void botFreeFly();
    void botFire( );
    void botSpringArmChged( const struct FInputActionValue& val );
	


    private:

    float ZMovementTotalTime;
    float TiltCurrVal;
    uint32 moveWithHitsCnt;

    FVector speedCheckPosSum;
    FVector speedcheckLastPos;
    uint32 speedCheckFrameCnt;
    float speedCheckLastTS;

	
};



