// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
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

	//DefaultMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("MeshForCells"));
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
		FBox MeshBounds = DefaultMesh->GetBounds().GetBox();
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < Columns; j++)
			{
				FVector location = FVector(j*MeshBounds.Max.X * 2, i*MeshBounds.Max.Y * 2, 0);

				//Create Collision Subobject
				UBoxComponent* CellCollision = NewObject<UBoxComponent>(this);
				CellCollision->SetupAttachment(RootComponent);
				CellCollision->RegisterComponent();
				CellCollision->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				CellCollision->SetBoxExtent(MeshBounds.GetExtent());
				CellCollision->SetRelativeLocation(location);
				//CellCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				CellCollision->SetCollisionProfileName(TEXT("WorldStatic"));
				CellCollision->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Block);
				CellCollision->OnComponentBeginOverlap.AddDynamic(this, &AGrid::OnOverlapBegin);
				CellCollision->OnComponentEndOverlap.AddDynamic(this, &AGrid::OnOverlapEnd);

				//Create Mesh
				UStaticMeshComponent* CellMesh = NewObject<UStaticMeshComponent>(this);
				CellMesh->SetupAttachment(CellCollision);
				CellMesh->RegisterComponent();
				CellMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
				CellMesh->SetStaticMesh(DefaultMesh);
				CellMesh->SetMaterial(0, ChooseCellMaterial(i, j));
				FVector Box = CellCollision->Bounds.BoxExtent;
				CellMesh->SetRelativeLocation(FVector(0, 0, -Box.Z));
			

				//CellMesh->SetWorldLocation(FVector(Origin.X, Origin.Y - Box.Y, Origin.Z - Box.Z));
				CellMesh->SetCollisionProfileName(TEXT("OverlapAll"));
				//CellMesh->SetCollisionObjectType(ECollisionChannel::ECC_Visibility);
				//CellMesh->BodyInstance.SetResponseToChannel(ECC_WorldStatic, ECR_Overlap);
			}
		}
	}
}

UMaterialInterface* AGrid::ChooseCellMaterial(int row, int column)
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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s overlapped with: %s"), *OverlappedComp->GetName(), *OtherActor->GetName()));
	}
}

void AGrid::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s not any more overlapped with: %s"), *OverlappedComp->GetName(), *OtherActor->GetName()));
	}
}
