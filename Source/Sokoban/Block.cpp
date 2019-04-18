// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SnapToGridComponent.h"

// Sets default values
ABlock::ABlock(const FObjectInitializer &ObjectInitializer)
		: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a sphere that reacts to physics
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	BoxCollision->SetCollisionProfileName(TEXT("BlockAll"));
	//BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxCollision->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Block);
	RootComponent = BoxCollision;

	// Create and position a mesh component so we can see where our sphere is
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereVisualRepresentation"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	//MeshComponent->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	//Create Snap Component
	SnapComponent = CreateDefaultSubobject<USnapToGridComponent>(TEXT("SnapComponent"));
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlock::OnConstruction(const FTransform & Transform) {
	//Set Collision size according to the mesh
	BoxCollision->SetBoxExtent(MeshComponent->Bounds.BoxExtent);

	//Set mesh location according to collision
	FVector Box = MeshComponent->Bounds.BoxExtent;
	MeshComponent->SetRelativeLocation(FVector(0, 0, -Box.Z));

	//Snap to grid cells
	SnapComponent->Snap();
}
