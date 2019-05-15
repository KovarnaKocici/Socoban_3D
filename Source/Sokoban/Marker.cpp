// Fill out your copyright notice in the Description page of Project Settings.


#include "Marker.h"
#include "Sokoban.h"
#include "Block.h"
#include "SokobanGameModeBase.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"

// Sets default values
AMarker::AMarker(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	TopCenterCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollisionComponent"));
	TopCenterCollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	TopCenterCollisionComponent->SetupAttachment(RootComponent);

	TopCenterCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMarker::OnOverlapBegin);
	TopCenterCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMarker::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AMarker::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMarker::OnConstruction(const FTransform & Transform) {
	if (DefaultMesh) {
		Super::OnConstruction(Transform);

		int collisionSize = 1;
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		TopCenterCollisionComponent->SetBoxExtent(FVector(collisionSize, collisionSize, collisionSize));
		TopCenterCollisionComponent->SetRelativeLocation(FVector(0.f, 0.f, MeshBounds.Z * 2 + collisionSize*2));
	}
}

void AMarker::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollision, Log, TEXT("%s overlapped with %s."), *OverlappedComp->GetName(), *OtherActor->GetName());
		ABlock* block = Cast<ABlock>(OtherActor);
		if (block) {
			ASokobanGameModeBase* gm = (ASokobanGameModeBase*)GetWorld()->GetAuthGameMode();
			gm->CurrNumMarkers += 1;
		}
	}
}

void  AMarker::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s not any more overlapped with %s."), *OverlappedComp->GetName(), *OtherActor->GetName());
		ABlock* block = Cast<ABlock>(OtherActor);
		if (block) {
			ASokobanGameModeBase* gm = (ASokobanGameModeBase*)GetWorld()->GetAuthGameMode();
			gm->CurrNumMarkers -= 1;
		}
	}
}