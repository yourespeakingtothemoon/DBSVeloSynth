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

	Node_FrontRight= CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FrontRight"));
	Node_FrontRight->SetupAttachment(Mesh_Body, FName("FR"));
	Node_FrontRight->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Node_FrontLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_FrontLeft"));
	Node_FrontLeft->SetupAttachment(Mesh_Body, FName("FL"));
	Node_FrontLeft->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Node_BackRight = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_BackRight"));
	Node_BackRight->SetupAttachment(Mesh_Body, FName("BR"));
	Node_BackRight->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	Node_BackLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow_BackLeft"));
	Node_BackLeft->SetupAttachment(Mesh_Body, FName("BL"));
	Node_BackLeft->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

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
if (HitResult.IsValidBlockingHit())
{
	float curLength = HitResult.Distance - FieldRadius;
	SpringLength[NodeIdx] = FMath::Clamp(curLength, MinSpringLength, MaxSpringLength);
	float springOffset = RestLength - SpringLength[NodeIdx];
float springForce = SpringConstant * springOffset;
FVector springForceVector = GetActorUpVector() * springForce;
Mesh_Body->AddForceAtLocation(springForceVector, node->GetComponentLocation());


}
else
{
	SpringLength[NodeIdx] = MaxSpringLength;
}


}

