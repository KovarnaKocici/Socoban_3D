// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Sokoban.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"


// Sets default values
AGrid::AGrid(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SceneComponent = RootComponent;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrid::OnConstruction(const FTransform & Transform) {
	if (DefaultMesh) {
		FBox MeshBounds = DefaultMesh->GetBoundingBox();
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				FVector CellLocation = FVector(j*MeshBounds.Max.X * 2, i*MeshBounds.Max.Y * 2, 0);

				//Create Mesh
				UStaticMeshComponent* CellMesh = NewObject<UStaticMeshComponent>(this);
				CellMesh->SetupAttachment(RootComponent);
				CellMesh->RegisterComponent();
				CellMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				CellMesh->SetStaticMesh(DefaultMesh);
				CellMesh->SetMaterial(0, SwitchCellMaterial(i, j));
				CellMesh->SetRelativeLocation(CellLocation);
				CellMesh->OnComponentBeginOverlap.AddDynamic(this, &AGrid::OnOverlapBegin);
				CellMesh->OnComponentEndOverlap.AddDynamic(this, &AGrid::OnOverlapEnd);
				CellMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				CellMesh->SetCollisionObjectType(ECC_WorldStatic);
				CellMesh->SetCollisionResponseToAllChannels(ECR_Block);
				CellMesh->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Overlap);
				
			}
		}
	}
}

UMaterialInterface* AGrid::SwitchCellMaterial(int row, int column)
{
	int material = (row % 2 + column % 2) % 2;
	switch (material)
	{
		case 0:return Material1;
		case 1:	return Material2;
		default: return NULL;
	}
}

void AGrid::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollision, Log, TEXT("%s overlapped with %s"), *OverlappedComp->GetName(), *OtherActor->GetName());
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s overlapped with %s"), *OverlappedComp->GetName(), *OtherActor->GetName()));
	}
}

void AGrid::OnOverlapEnd(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s not any more overlapped with %s"), *OverlappedComp->GetName(), *OtherActor->GetName());
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s not any more overlapped with %s"), *OverlappedComp->GetName(), *OtherActor->GetName()));
	}
}