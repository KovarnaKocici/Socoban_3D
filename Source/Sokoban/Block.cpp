// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Sokoban.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "TPPlayerMovementComponent.h"

// Sets default values
ABlock::ABlock(const FObjectInitializer &ObjectInitializer)
		: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlock::OnOverlapBegin);

	TopCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollisionComponent"));
	TopCollisionComponent->SetCollisionProfileName(TEXT("Snap"));
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

void  ABlock::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollision, Log, TEXT("%s overlapped with %s."), *OverlappedComp->GetName(), *OtherActor->GetName());
		ABlock* block = Cast<ABlock>(OtherActor);
		if (block)
		{
			MovementComponent->ReverseMove();
			IsHitted = true;
		}
	}
}
