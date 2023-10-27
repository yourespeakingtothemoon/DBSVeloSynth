// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/DBSVS_VehicleBase.h"

// Sets default values
ADBSVS_VehicleBase::ADBSVS_VehicleBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADBSVS_VehicleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADBSVS_VehicleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADBSVS_VehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

