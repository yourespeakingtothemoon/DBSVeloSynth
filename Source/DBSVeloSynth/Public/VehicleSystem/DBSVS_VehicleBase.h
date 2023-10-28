// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DBSVS_VehicleBase.generated.h"

UCLASS()
class DBSVELOSYNTH_API ADBSVS_VehicleBase : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh_Body;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* Node_FrontRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* Node_FrontLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* Node_BackRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* Node_BackLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	//Exposed Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RestLength = 50.0f;
UPROPERTY(EditAnywhere, BlueprintReadOnly)
float SpringTravelLength = 10;
UPROPERTY(EditAnywhere, BlueprintReadOnly)
float FieldRadius = 34;
UPROPERTY(EditAnywhere, BlueprintReadOnly)
float SpringConstant = 500;

UPROPERTY(EditAnywhere, BlueprintReadOnly)
FName TraceTag = "SuspensionTrace";


	// Sets default values for this pawn's properties
	ADBSVS_VehicleBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	TArray<UArrowComponent*> Nodes;
	float MinSpringLength;
float MaxSpringLength;
float SpringLength[4] = {0,0,0,0};
FCollisionQueryParams TraceCollisionParams;


void UpdateSuspension(int NodeIdx, float DeltaTime);

};
