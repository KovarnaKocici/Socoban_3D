// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Sokoban.h"
#include "SnapToGridComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABlock::ABlock(const FObjectInitializer &ObjectInitializer)
		: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->OnComponentHit.AddDynamic(this, &ABlock::OnCompHit);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	//Mesh as Root
	RootComponent = MeshComponent;

	TopCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TopCollisionComponent"));
	TopCollisionComponent->SetCollisionProfileName(TEXT("Snap"));
	TopCollisionComponent->OnComponentCollisionSettingsChangedEvent;
	TopCollisionComponent->SetupAttachment(RootComponent);

	InnerCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerCollisionComponent"));
	InnerCollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	InnerCollisionComponent->OnComponentCollisionSettingsChangedEvent;
	InnerCollisionComponent->SetupAttachment(RootComponent);

	//Create Snap Component
	SnapComponent = CreateDefaultSubobject<USnapToGridComponent>(TEXT("SnapComponent"));

	//Initialize delta transform 
	DeltaMove = FVector(0, 0, 0);
	DeltaSize = FVector(0, 0, 0);
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
		int collisionH = 1;
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		MeshComponent->SetStaticMesh(DefaultMesh);
		MeshComponent->SetMaterial(0, DefaultMaterial);

		TopCollisionComponent->SetBoxExtent(FVector(MeshBounds.X, MeshBounds.Y, collisionH));
		TopCollisionComponent->SetRelativeLocation(FVector(0, 0, MeshBounds.Z * 2 - collisionH));

		InnerCollisionComponent->SetBoxExtent(MeshBounds*0.95);
		InnerCollisionComponent->SetRelativeLocation(FVector(0, 0, MeshBounds.Z));

	}
}

void ABlock::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogSnapping, Log, TEXT(" DeltaTranslation is: %s"), *DeltaTranslation.ToString());
	ApplyTranslation(DeltaTranslation);
}

void ABlock::EditorApplyScale(const FVector & DeltaScale, const FVector * PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogSnapping, Log, TEXT(" DeltaScale is: %s"), *DeltaScale.ToString());

	FVector CurrScale = GetActorScale3D();
	FVector NewScale = FVector(0,0,0);

	DeltaSize.X += DeltaScale.X;
	NewScale.X = CurrScale.X + DeltaScale.X;

	DeltaSize.Y += DeltaScale.Y;
	NewScale.Y = CurrScale.Y + DeltaScale.Y;

	DeltaSize.Z += DeltaScale.Z;
	NewScale.Z = CurrScale.Z + DeltaScale.Z;

	SetActorScale3D(NewScale);
	ApplyTranslation(FVector(0, 0, DeltaScale.Z));

}

void ABlock::PostEditMove(bool bFinished)
{
	if (bFinished) {
		FVector CurrLocation = this->GetActorLocation();
		FVector CurrScale = this->GetActorScale3D();

		TArray<TEnumAsByte<enum EObjectTypeQuery>>  ActorsToOverlap;
		ActorsToOverlap.Add(EObjectTypeQuery::ObjectTypeQuery1); //WorldStatic
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		TArray<AActor*> OutActors;
		
		//Check move collision
		if (!UKismetSystemLibrary::ComponentOverlapActors(InnerCollisionComponent, InnerCollisionComponent->GetComponentTransform(), ActorsToOverlap, NULL, IgnoreActors, OutActors))
		{
			if (DeltaMove.X != 0 || DeltaMove.Y != 0) {
				UPrimitiveComponent* cell = SnapComponent->GetCurrCell();
				if (cell)
					SnapComponent->SnapToCell(cell);
			}
			if (DeltaMove.Z != 0)
				SnapComponent->SnapToFloor(DeltaMove.Z / fabs(DeltaMove.Z));
		}
		else
		{
			UE_LOG(LogCollide, Log, TEXT("%s overlaps."), *this->GetName());
			SetActorLocation(CurrLocation - DeltaMove);
			SetActorScale3D(CurrScale - DeltaSize);
		}

		DeltaMove = FVector(0, 0, 0);
		DeltaSize = FVector(0, 0, 0);
	}
}

void ABlock::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		UE_LOG(LogCollide, Log, TEXT("%s hits %s"), *this->GetName(), *OtherActor->GetName());
	}
}

void ABlock::ApplyTranslation(const FVector & DeltaTranslation) {

	FVector CurrLocation = GetActorLocation();
	FVector NewLocation = FVector(0,0,0);

	DeltaMove.X += DeltaTranslation.X;
	NewLocation.X = CurrLocation.X + DeltaTranslation.X;

	DeltaMove.Y += DeltaTranslation.Y;
	NewLocation.Y = CurrLocation.Y + DeltaTranslation.Y;

	DeltaMove.Z += DeltaTranslation.Z;
	NewLocation.Z = CurrLocation.Z + DeltaTranslation.Z;

	this->SetActorLocation(NewLocation);
}