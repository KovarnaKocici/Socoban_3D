// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Sokoban.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"


// Sets default values
ABlock::ABlock(const FObjectInitializer &ObjectInitializer)
		: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->OnComponentHit.AddDynamic(this, &ABlock::OnCompHit);

	TopCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollisionComponent"));
	TopCollisionComponent->SetCollisionProfileName(TEXT("Snap"));
	TopCollisionComponent->OnComponentCollisionSettingsChangedEvent;
	TopCollisionComponent->SetupAttachment(RootComponent);

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
		Super::OnConstruction(Transform);

		int collisionH = 1;
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		TopCollisionComponent->SetBoxExtent(FVector(MeshBounds.X, MeshBounds.Y, collisionH));
		TopCollisionComponent->SetRelativeLocation(FVector(0.f, 0.f, MeshBounds.Z * 2 - collisionH));
	}

}

void ABlock::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName());
	}
}
