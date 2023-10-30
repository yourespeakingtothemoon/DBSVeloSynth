// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//include UKismetMathLibrary for FInterpTo
#include "Kismet/KismetMathLibrary.h"
#include "DBSVS_VehicleBase.generated.h"

UCLASS()
class DBSVELOSYNTH_API ADBSVS_VehicleBase : public APawn
{
	GENERATED_BODY()

public:

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Vehicle Mesh")
	UStaticMeshComponent* Mesh_Body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle Mesh")
	UArrowComponent* Node_FrontRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle Mesh")
	UArrowComponent* Node_FrontLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle Mesh")
	UArrowComponent* Node_BackRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle Mesh")
	UArrowComponent* Node_BackLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Setup")
	UCameraComponent* Camera;

	//Exposed Variables

	//Category: Suspension
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Suspension")
	float RestLength = 50.0f;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Suspension")
float SpringTravelLength = 35;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Suspension")
float FieldRadius = 30;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Suspension")
float SpringConstant = 15000;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Suspension")
float DamperConstant = 100;
//Category: Thrust
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Thrust")
float ThrustConstant = 300000;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Thrust")
float BrakeConstant = 50;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Thrust")
float ThrustTriggerInterpolationSpeed = 4;
//Category: Steering

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Steering")
float MaxSteeringAngle = 15;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Steering")
float SteerTriggerInterpolationSpeed = 4;
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hover Physics | Steering")
float FrictionConstant = 400;


UPROPERTY(EditAnywhere, BlueprintReadOnly)
FName TraceTag = "SuspensionTrace";


	// Sets default values for this pawn's properties
	ADBSVS_VehicleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	float ThrustAxis = 0;
	float SteerAxis = 0;

	UPROPERTY(BlueprintReadOnly)
	bool isBraking = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AddThrust(float AxisValue);
	UFUNCTION(BlueprintCallable)
	void Steer(float AxisValue);
	UFUNCTION(BlueprintCallable)
	void Brake(bool value);


private:
	TArray<UArrowComponent*> Nodes;
	float MinSpringLength;
float MaxSpringLength;
float SpringLength[4] = {0,0,0,0};
FCollisionQueryParams TraceCollisionParams;


void UpdateSuspension(int NodeIdx, float DeltaTime);



};
