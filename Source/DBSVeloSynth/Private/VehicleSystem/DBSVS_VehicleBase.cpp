// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleSystem/DBSVS_VehicleBase.h"

// Sets default values
ADBSVS_VehicleBase::ADBSVS_VehicleBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh_Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh_Body"));
	RootComponent = Mesh_Body;
	if(UStaticMesh* CraftMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("'StaticMesh' /Game/Assets/Meshes/SM_Craft"))))
{
		Mesh_Body->SetStaticMesh(CraftMesh);
	}

	//set mesh default settings
	Mesh_Body->SetSimulatePhysics(true);
	Mesh_Body->SetMassOverrideInKg(NAME_None, 250.0f, true);

	Node_FrontRight= CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FrontRight"));
	Node_FrontRight->SetupAttachment(Mesh_Body, FName("Node_FR"));
	Node_FrontRight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Node_FrontLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FrontLeft"));
	Node_FrontLeft->SetupAttachment(Mesh_Body, FName("Node_FL"));
	Node_FrontLeft->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Node_BackRight = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_BackRight"));
	Node_BackRight->SetupAttachment(Mesh_Body, FName("Node_BR"));
	Node_BackRight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Node_BackLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_BackLeft"));
	Node_BackLeft->SetupAttachment(Mesh_Body, FName("Node_BL"));
	Node_BackLeft->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Mesh_Body);
	//set length of spring arm from BP
	//and angle of spring arm from BP
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	//set camera settings from BP

}

// Called when the game starts or when spawned
void ADBSVS_VehicleBase::BeginPlay()
{
	Super::BeginPlay();

	Nodes={ Node_FrontRight, Node_FrontLeft, Node_BackRight, Node_BackLeft };
	MinSpringLength = RestLength - SpringTravelLength;
	MaxSpringLength = RestLength + SpringTravelLength;

	TraceCollisionParams.AddIgnoredActor(this);
	TraceCollisionParams.bDebugQuery = true;
	TraceCollisionParams.TraceTag = TraceTag;
}

void ADBSVS_VehicleBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Mesh_Body->SetSimulatePhysics(true);
	Mesh_Body->SetMassOverrideInKg(NAME_None, 1000.0f, true);

}

// Called every frame
void ADBSVS_VehicleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (int i = 0; i < Nodes.Num(); i++)
	{
		UpdateSuspension(i, DeltaTime);
	}


}

// Called to bind functionality to input
void ADBSVS_VehicleBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADBSVS_VehicleBase::AddThrust(float AxisValue)
{
	
	ThrustAxis = FMath::FInterpTo(ThrustAxis, AxisValue, GetWorld()->GetDeltaSeconds(), ThrustTriggerInterpolationSpeed);
//ThrustAxis = AxisValue;
}

void ADBSVS_VehicleBase::Steer(float AxisValue)
{
SteerAxis = FMath::FInterpTo(SteerAxis, AxisValue, GetWorld()->GetDeltaSeconds(), SteerTriggerInterpolationSpeed);
}

void ADBSVS_VehicleBase::Brake(bool value)
{
	isBraking = value;
}



void ADBSVS_VehicleBase::UpdateSuspension(int NodeIdx, float DeltaTime)
{
	if(!Nodes.IsValidIndex(NodeIdx))
	{
		return;
	}
	auto node = Nodes[NodeIdx];
	FHitResult HitResult;
FVector Start = node->GetComponentLocation();
FVector End = node->GetForwardVector()*(MaxSpringLength+FieldRadius)+Start;


GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Camera,TraceCollisionParams,FCollisionResponseParams());
float prevLength = SpringLength[NodeIdx];

FVector wLinVelocity = Mesh_Body->GetPhysicsLinearVelocity();
FVector lLinVelocity = UKismetMathLibrary::InverseTransformDirection(Mesh_Body->GetComponentTransform(), wLinVelocity);


if (HitResult.IsValidBlockingHit())
{
	float curLength = HitResult.Distance - FieldRadius;
	SpringLength[NodeIdx] = FMath::Clamp(curLength, MinSpringLength, MaxSpringLength);
	float springOffset = RestLength - SpringLength[NodeIdx];
	float springVelocity = (springOffset - prevLength) / DeltaTime;
	float damperForce = DamperConstant * springVelocity;
float springForce = SpringConstant * springOffset;
FVector springForceVector = GetActorUpVector() * (springForce-damperForce);
Mesh_Body->AddForceAtLocation(springForceVector, node->GetComponentLocation());


}
else
{
	SpringLength[NodeIdx] = MaxSpringLength;
}

float CurrentSteerAngle = UKismetMathLibrary::MapRangeClamped(SteerAxis, -1.0f, 1.0f, -MaxSteeringAngle, MaxSteeringAngle);
if (wLinVelocity.SizeSquared() > 1)
{
	Mesh_Body->AddTorqueInDegrees(FVector(0.0f, 0.0f, CurrentSteerAngle), NAME_None,true);
	//tilt destroys lives Mesh_Body->AddTorqueInDegrees(FVector(0, SteerAxis * 15, 0), NAME_None, true);
}
FVector FrictionVec = FVector::ZeroVector;
if (UKismetMathLibrary::Abs(lLinVelocity.Y) > 2)
{
	FrictionVec= Mesh_Body->GetRightVector() * lLinVelocity.Y * -1.0f * FrictionConstant;
}

FVector ThrustForceVector = GetActorForwardVector() * ThrustConstant*ThrustAxis + FrictionVec;

if (isBraking)
{
	ThrustForceVector = wLinVelocity * -1.0f * BrakeConstant;

}
Mesh_Body->AddForce(ThrustForceVector);

}

