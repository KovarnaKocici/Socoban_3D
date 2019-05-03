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

void USnapToGridComponent::Snap()
{

	if(!SnapToCellCenter(1000, ETraceDirection::TopDown))
	SnapToCellCenter(1000, ETraceDirection::DownTop);

	SnapToFloor(1000, ETraceDirection::TopDown);
		SnapToFloor(1000, ETraceDirection::DownTop);
}

FVector USnapToGridComponent::NewLocationRelativeToObject(ECollisionChannel Type, FVector Start, FVector  End, int direction) {
	TArray<FHitResult> OutHit;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FVector OwnerBounds = GetOwner()->GetComponentsBoundingBox().GetExtent();

	if (GetWorld()->SweepMultiByObjectType(OutHit, Start, End, GetOwner()->GetActorRotation().Quaternion(), Type, FCollisionShape::MakeBox(OwnerBounds), CollisionParams))
	{
		FHitResult Hit;

		if (direction == ETraceDirection::TopDown)
			Hit = OutHit[0];
		else
			Hit = OutHit[OutHit.Num()-1];

		UE_LOG(LogSnapping, Log, TEXT("Hit actor is : %s. Trace distance is : %f. Direction is : %d."), *(Hit.GetActor())->GetName(), Hit.Distance , direction);
		DrawDebugBox(GetWorld(), Hit.Location, OwnerBounds, direction == ETraceDirection::TopDown ? FColor::Red : FColor::Blue, false, 3.0f, 0, 3.0f);
		if(direction == ETraceDirection::TopDown) return Hit.Location - FVector(0, 0, OwnerBounds.Z*2);
		else return Hit.Location + FVector(0, 0, OwnerBounds.Z);
	}
	return Start;
}

bool USnapToGridComponent::SnapToFloor(float traceLength, int direction)
{
	FVector trace = FVector(0, 0, traceLength * direction);
	FVector CurrLocation = GetOwner()->GetActorLocation();
	FVector NewLocationZ = NewLocationRelativeToObject(ECC_WorldStatic, CurrLocation, CurrLocation + trace, direction);
	if (CurrLocation != NewLocationZ)
	{
		GetOwner()->SetActorLocation(NewLocationZ, false);
		return true;
	}
	return false;
}

bool USnapToGridComponent::SnapToCellCenter(float traceLength, int direction)
{
	FVector trace = FVector(0, 0, traceLength*direction);
	FVector Start = GetOwner()->GetActorLocation();

	TArray<FHitResult> OutHit;
	FVector End;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	End = Start + trace;

	if (GetWorld()->LineTraceMultiByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams))
	{
		FHitResult Hit;

		for (int i = 0; i < OutHit.Num(); i++)
		{
			if (OutHit[i].GetActor()->IsA(AGrid::StaticClass()))
				Hit = OutHit[i];
			break;
		}
		if (Hit.GetActor()==NULL) return false;

		UE_LOG(LogSnapping, Log, TEXT("Hit component is : %s."), *(Hit.GetComponent())->GetName());
		DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.Location, direction == ETraceDirection::TopDown ? FColor::Red : FColor::Blue, false, 3.0f, 0, 3.0f);

		FVector UpdatedCurrLocation = GetOwner()->GetActorLocation();
		FVector HitComponentOrigin = Hit.GetComponent()->Bounds.Origin;
		FVector NewLocationXY = FVector(HitComponentOrigin.X, HitComponentOrigin.Y, UpdatedCurrLocation.Z);
		GetOwner()->SetActorLocation(NewLocationXY, false);
		return true;
	}
	return false;
}

