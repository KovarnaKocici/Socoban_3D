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
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ABlock::OnOverlapBegin);

	TopCoverCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollisionComponent"));
	TopCoverCollisionComponent->SetCollisionProfileName(TEXT("Snap"));
	TopCoverCollisionComponent->SetupAttachment(RootComponent);

	// Create an instance of our movement component, and tell it to update the root.
	MovementComponent = CreateDefaultSubobject<UTPPlayerMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->SetAlpha(0.025f);

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

UPawnMovementComponent* ABlock::GetMovementComponent() const
{
	return MovementComponent;
}

void ABlock::OnConstruction(const FTransform & Transform) {
	if (DefaultMesh) {
		Super::OnConstruction(Transform);

		int collisionH = 1;
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		TopCoverCollisionComponent->SetBoxExtent(FVector(MeshBounds.X, MeshBounds.Y, collisionH));
		TopCoverCollisionComponent->SetRelativeLocation(FVector(0.f, 0.f, MeshBounds.Z * 2 - collisionH));
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
			UE_LOG(LogMovement, Log, TEXT("Reverse move in Box overlap."));
		}
	}
}
