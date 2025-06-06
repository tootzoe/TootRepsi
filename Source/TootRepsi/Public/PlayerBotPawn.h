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
    //
    UPROPERTY( Replicated  )
    bool bCanShoot;

    UPROPERTY(EditAnywhere, Category="TOOT")
    float ShootingInterval;


    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    FVector MuzzleOffset;
    UPROPERTY(EditAnywhere, Category="TOOT")
    TSubclassOf<class AFlybotBasicShot> ShotSubClass;
    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    TSubclassOf<class UMainPlayerHUD> MainPlayerClass;
    UPROPERTY( )
    class UMainPlayerHUD* MainPlayerHUD;

    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float MaxHealth;
    UPROPERTY(ReplicatedUsing = OnRepHealth )
    float Health;
    //
    UPROPERTY( ReplicatedUsing=OnRep_Color,  BlueprintReadOnly, Transient, Category="TOOT")
    FLinearColor currColor;
    //
    // UPROPERTY(EditDefaultsOnly, Category="TOOT|Touch")
    // class UTouchInterface* PawnTouchInterface;

    UPROPERTY(EditDefaultsOnly, Category="TOOT|Touch")
      TSoftObjectPtr<class UTouchInterface> PawnTouchInterface;
    UPROPERTY(EditDefaultsOnly, Category="TOOT|Touch")
    TSoftObjectPtr<class UTouchInterface> PawnTouchInterface2;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    UFUNCTION(Server, Unreliable, Category = "TOOT")
    void Svr_UpdPawnTransform(FTransform Transform);
    UFUNCTION(Server, Reliable, Category = "TOOT")
    void Svr_UpdShooting(bool bNewShooting);
    //
    UFUNCTION(Client, Unreliable, Category = "TOOT")
    void Cli_UpdPawnTransform(FTransform Transform);

    UFUNCTION( )
    void OnRepHealth();
    UFUNCTION( )
    void OnRep_Color();



    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

    void updHealth(float healthDelta);


    //
    UFUNCTION()
        void OnOverlapBegin1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

        /** called when something leaves the sphere component */
        UFUNCTION()
        void OnOverlapEnd1(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


        UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                   FVector NormalImpulse, const FHitResult& Hit);

      //  UFUNCTION()
      //  void OnHit2(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

        public:
        void auth_setPlayerColor(const FLinearColor& cr);

protected:
    // Called when the game starts or when spawned
     virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    private:


    void botMove(const struct FInputActionValue& val);
    void botRotate(const struct FInputActionValue& val);
    void botFreeFly();
    void botFire(const FInputActionValue &val);
    void botSpringArmChged( const struct FInputActionValue& val );
	


    private:

    float ZMovementTotalTime;
    float TiltCurrVal;
    uint32 moveWithHitsCnt;

    FVector speedCheckPosSum;
    FVector speedcheckLastPos;
    uint32 speedCheckFrameCnt;
    float speedCheckLastTS;

    float shootingLastTS;

    //
    UPROPERTY(EditAnywhere, Category="TOOT")
    float MaxPower;
    float Power;
    UPROPERTY(EditAnywhere, Category="TOOT")
    float PowerRegenerateRate;
    void RegeneratePower();

    void tryShooting();

    void switchTouchInterface(int32 idx);


 };



