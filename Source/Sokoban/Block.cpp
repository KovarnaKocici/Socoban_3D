// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Sokoban.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "SnapToGridComponent.h"

// Sets default values
ABlock::ABlock(const FObjectInitializer &ObjectInitializer)
		: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->OnComponentHit.AddDynamic(this, &ABlock::OnCompHit);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);

	//Box Collision as Root
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

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
	if (DefaultMesh) {
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		BoxComponent->SetBoxExtent(MeshBounds);

		MeshComponent->SetStaticMesh(DefaultMesh);
		MeshComponent->SetMaterial(0, DefaultMaterial);
		MeshComponent->SetRelativeLocation(FVector(0, 0, -MeshBounds.Z));
	}

	SnapComponent->Snap();
}

void ABlock::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName());
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName()));
	}
}