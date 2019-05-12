// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapToGridComponent.h"
#include "Sokoban.h"
#include "Grid.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UObject/ObjectMacros.h"

// Sets default values for this component's properties
USnapToGridComponent::USnapToGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

// Called when the game starts
void USnapToGridComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void USnapToGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USnapToGridComponent::SetTraceLength(float Length) 
{
	if (Length > 0)
		TraceLength = Length;
}

float USnapToGridComponent::GetTraceLength() 
{
	return TraceLength;
}

UPrimitiveComponent* USnapToGridComponent::GetCellByLocation(FVector Location) {
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FVector Start = Location;
	FVector End = Start + FVector(0, 0, -TraceLength);

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel2, CollisionParams)) //Grid
	{
		if (Hit.GetActor() != NULL) {
			UE_LOG(LogSnapping, Log, TEXT("Hit component is : %s."), *(Hit.GetComponent())->GetName());
			DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.Location, FColor::Green, false, 3.0f, 0, 3.0f);
			return Hit.GetComponent();
		}
	}
	return NULL;
}

UPrimitiveComponent* USnapToGridComponent::GetCurrCell() {
	return GetCellByLocation(GetOwner()->GetActorLocation());
}

bool USnapToGridComponent::NewLocationXYRelToGrid(UPrimitiveComponent* cell, FVector& location) {
	if (cell != NULL) {
		FVector HitComponentOrigin = cell->Bounds.Origin;
		location = FVector(HitComponentOrigin.X, HitComponentOrigin.Y, 0);
		return true;
	}
	return false;
}

bool USnapToGridComponent::NewLocationZRelToActors(FVector Start, FVector  End, int direction, FVector& location) {
	TArray<FHitResult> OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FVector OwnerBounds = GetOwner()->GetComponentsBoundingBox().GetExtent();

	if (GetWorld()->SweepMultiByChannel(OutHit, Start, End, GetOwner()->GetActorRotation().Quaternion(), ECC_GameTraceChannel1, FCollisionShape::MakeBox(OwnerBounds), CollisionParams))
	{
		FHitResult	Hit = OutHit[0];

		if (Hit.ImpactPoint.Z != Start.Z ) {
			UE_LOG(LogSnapping, Log, TEXT("Hit actor is : %s. Hit component is : %s. ImpactPoint is : %f. Direction is : %d."), *(Hit.GetActor())->GetName(), *(Hit.GetComponent())->GetName(), Hit.ImpactPoint.Z, direction);
			DrawDebugBox(GetWorld(), Hit.Location, OwnerBounds, direction == ETraceDirection::TopDown ? FColor::Red : FColor::Blue, false, 3.0f, 0, 3.0f);
			location = FVector(0, 0, Hit.ImpactPoint.Z);
			return true;
		}
	}
	return false;
}

bool USnapToGridComponent::SnapToCell(UPrimitiveComponent* cell)
{
	FVector CurrLocation = GetOwner()->GetActorLocation();
	FVector NewLocationXY; 
	if (NewLocationXYRelToGrid(cell, NewLocationXY)) {
		if (CurrLocation.X != NewLocationXY.X || CurrLocation.Y != NewLocationXY.Y) {
			GetOwner()->SetActorLocation(FVector(NewLocationXY.X, NewLocationXY.Y, CurrLocation.Z));
				return true;
		}
	}
	return false;
}

bool USnapToGridComponent::SnapToFloor(int direction)
{
	FVector trace = FVector(0, 0, TraceLength * direction);
	FVector CurrLocation = GetOwner()->GetActorLocation();
	FVector NewLocationZ;
	if (NewLocationZRelToActors( CurrLocation, CurrLocation + trace, direction, NewLocationZ)) {
		if (CurrLocation.Z != NewLocationZ.Z)
		{
			GetOwner()->SetActorLocation(FVector(CurrLocation.X, CurrLocation.Y, NewLocationZ.Z));
			return true;
		}
	}
	return false;
}


