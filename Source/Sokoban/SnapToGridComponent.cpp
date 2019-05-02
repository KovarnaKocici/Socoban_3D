// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapToGridComponent.h"
#include "Sokoban.h"
#include "Grid.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
	SnapToCellCenter(1000);
	//TopDown
	if (!SnapToFloor(1000, -1)) {
		//DownTop
		SnapToFloor(1000, 1);
	}
}

float USnapToGridComponent::CalcDistanceToObject(ECollisionChannel Type, FVector Start, FVector  End, int index) { // index == 0 first, index == 1 last
	TArray<FHitResult> OutHit;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceMultiByObjectType(OutHit, Start, End, Type, CollisionParams))
	{
		FHitResult Hit;
		float dist;
		if (index == 0) {
			Hit = OutHit[0];
			dist = Hit.Distance;
		}
		else
		{
			Hit = OutHit[OutHit.Num()-1];
			dist = Hit.Distance;
		}
		DrawDebugLine(GetWorld(),Hit.TraceStart, Hit.Location, index == 0 ? FColor::Red : FColor::Blue,false, 5.0f, 0, 7.0f);
		UE_LOG(LogSnapping, Log, TEXT("Hit actor is : %s. Trace distance is : %f. Direction is : %d."), *(Hit.GetActor())->GetName(), dist, index);
		return dist;
	}
	return 0;
}

bool USnapToGridComponent::SnapToFloor(float traceLength, int direction)
{
	FVector trace = FVector(0, 0, traceLength * direction);
	FVector CurrLocation = GetOwner()->GetActorLocation();
	double distance = CalcDistanceToObject(ECC_WorldDynamic, CurrLocation, CurrLocation + trace, direction>0?1:0);
	//Snap to floor
	if (distance != 0) {
		if (direction > 0) distance = -distance;
		FVector NewLocationZ = FVector(CurrLocation.X, CurrLocation.Y, CurrLocation.Z - distance);
		GetOwner()->SetActorLocation(NewLocationZ, false);
		return true;
	}
	return false;
}

bool USnapToGridComponent::SnapToCellCenter(float traceLength)
{
	FVector trace = FVector(0, 0, -traceLength);
	FVector Start = GetOwner()->GetActorLocation();

	FHitResult OutHit;
	FVector End;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	End = Start + trace;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams))
	{
		//Print out the name of the traced actor
		UE_LOG(LogSnapping, Log, TEXT("Hit component is : %s."), *(OutHit.GetComponent())->GetName());

		//Snap to center of component that was hit
		FVector UpdatedCurrLocation = GetOwner()->GetActorLocation();
		FVector HitComponentOrigin = OutHit.GetComponent()->Bounds.Origin;
		FVector NewLocationXY = FVector(HitComponentOrigin.X, HitComponentOrigin.Y, UpdatedCurrLocation.Z);
		GetOwner()->SetActorLocation(NewLocationXY, false);
	}
	return true;
}

