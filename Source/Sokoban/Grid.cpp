// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Sokoban.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


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
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(controller)
		controller->ClientMessage(TEXT("GridBeforeIf"), TEXT("OnConstruction"), 500);
	if (DefaultMesh) {
		if (controller)
			controller->ClientMessage(TEXT("GridInIf"), TEXT("OnConstruction"), 500);
		FBox MeshBounds = DefaultMesh->GetBoundingBox();
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				FVector CellLocation = FVector(j*MeshBounds.Max.X * 2, i*MeshBounds.Max.Y * 2, 0);

				//Create Mesh
				UStaticMeshComponent* CellMesh = NewObject<UStaticMeshComponent>(this);
				CellMesh->SetupAttachment(RootComponent);NewObject<UBoxComponent>(this);
				CellMesh->RegisterComponent();
				CellMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				CellMesh->SetStaticMesh(DefaultMesh);
				CellMesh->SetMaterial(0, SwitchCellMaterial(i, j));
				CellMesh->SetRelativeLocation(CellLocation);
				CellMesh->SetCollisionProfileName(TEXT("Grid"));

				int collisionH = 1;

				UBoxComponent* CellCollision = NewObject<UBoxComponent>(this);
				CellCollision->SetupAttachment(CellMesh);
				CellCollision->RegisterComponent();
				CellCollision->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				CellCollision->SetBoxExtent(FVector(MeshBounds.GetExtent().X, MeshBounds.GetExtent().Y, collisionH));
				CellCollision->SetRelativeLocation(FVector(0, 0, MeshBounds.GetExtent().Z * 2 - collisionH));
				CellCollision->SetCollisionProfileName(TEXT("Snap"));
				CellCollision->SetGenerateOverlapEvents(true);
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
