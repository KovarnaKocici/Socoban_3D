// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPossession.h"
#include "Sokoban.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include "SnapToGridComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGridPossession::AGridPossession(const FObjectInitializer &ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	
	//Mesh as Root
	RootComponent = MeshComponent;

	InnerCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerCollisionComponent"));
	InnerCollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	InnerCollisionComponent->OnComponentCollisionSettingsChangedEvent;
	InnerCollisionComponent->SetupAttachment(RootComponent);

	//Create Snap Component
	SnapComponent = CreateDefaultSubobject<USnapToGridComponent>(TEXT("SnapComponent"));
	SnapComponent->SetTraceLength(1000.f);

}

// Called when the game starts or when spawned
void AGridPossession::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridPossession::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridPossession::OnConstruction(const FTransform & Transform) 
{
	if (DefaultMesh) {
		int collisionH = 1;
		FVector MeshBounds = DefaultMesh->GetBoundingBox().GetExtent();

		MeshComponent->SetStaticMesh(DefaultMesh);
		MeshComponent->SetMaterial(0, DefaultMaterial);

		InnerCollisionComponent->SetBoxExtent(MeshBounds - FVector(3.f, 3.f, 3.f));
		InnerCollisionComponent->SetRelativeLocation(FVector(0.f, 0.f, MeshBounds.Z));

	}
}

void AGridPossession::EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogSnapping, Log, TEXT(" DeltaTranslation is: %s"), *DeltaTranslation.ToString());
	ApplyTranslation(DeltaTranslation);
}

void AGridPossession::EditorApplyScale(const FVector & DeltaScale, const FVector * PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	UE_LOG(LogSnapping, Log, TEXT(" DeltaScale is: %s"), *DeltaScale.ToString());

	ApplyScale(DeltaScale);
	ApplyTranslation(FVector(0.f, 0.f, DeltaScale.Z));

}

//#if WITH_EDITOR
void AGridPossession::PostEditMove(bool bFinished)
{
	if (bFinished) {
		FVector CurrLocation = this->GetActorLocation();
		FVector CurrScale = this->GetActorScale3D();

		TArray<TEnumAsByte<enum EObjectTypeQuery>>  ActorsToOverlap;
		ActorsToOverlap.Add(EObjectTypeQuery::ObjectTypeQuery1); //WorldStatic
		ActorsToOverlap.Add(EObjectTypeQuery::ObjectTypeQuery3); //Pawn
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

		DeltaMove = FVector(0.f, 0.f, 0.f);
		DeltaSize = FVector(0.f, 0.f, 0.f);
	}
}
//#endif

void AGridPossession::ApplyTranslation(const FVector & DeltaTranslation) 
{

	FVector NewLocation = Apply(GetActorLocation(), DeltaMove, DeltaTranslation);
	SetActorLocation(NewLocation);
}

void AGridPossession::ApplyScale(const FVector & DeltaScale) 
{
	FVector NewScale = Apply(GetActorScale3D(), DeltaSize, DeltaScale);
	SetActorScale3D(NewScale);
}

FVector AGridPossession::Apply(const FVector & CurrValue, FVector & Accumulator, const FVector & Delta) 
{
	FVector NewValue = FVector(0.f, 0.f, 0.f);

	Accumulator.X += Delta.X;
	NewValue.X = CurrValue.X + Delta.X;

	Accumulator.Y += Delta.Y;
	NewValue.Y = CurrValue.Y + Delta.Y;

	Accumulator.Z += Delta.Z;
	NewValue.Z = CurrValue.Z + Delta.Z;

	return NewValue;
}

bool AGridPossession::ValidGridMovement(const FVector & StartLocation, FVector & DesiredMovement, int direction)
{
	//Check start location
	UPrimitiveComponent* cell = SnapComponent->GetCellByLocation(StartLocation);
	if (cell)
	{
		float accuracy = 0;

		if (direction == EMoveDirection::MoveForward)
		{
			if (FMath::IsNearlyEqual(FMath::Abs(GetActorRotation().Yaw), 90, 1.f))
				accuracy = cell->Bounds.BoxExtent.Y * 2;
			else
				accuracy = cell->Bounds.BoxExtent.X * 2;
		}
		else
		if (direction == EMoveDirection::MoveRight)
		{
			if (FMath::IsNearlyEqual(FMath::Abs(GetActorRotation().Yaw), 90, 1.f))
				accuracy = cell->Bounds.BoxExtent.X * 2;
			else
				accuracy = cell->Bounds.BoxExtent.Y * 2;
		}

		//Check end location
		if (SnapComponent->GetCellByLocation(StartLocation + DesiredMovement * accuracy))
		{
			DesiredMovement *= accuracy;
			return true;
		}
	}

	return false;
}