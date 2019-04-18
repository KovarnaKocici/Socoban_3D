// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapToGridComponent.h"
#include "Engine/World.h"
#include "Grid.h"

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
	FHitResult OutHit;
	FVector Start;
	FVector End;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FVector CurrLocation = GetOwner()->GetActorLocation();
	Start = CurrLocation;
	End = Start + FVector(0., 0., -500.);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_WorldStatic, CollisionParams))
	{
		//Print out the name of the traced actor
		if (OutHit.GetActor())
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit actor is : %s . Trace Distance is : %f ."), *(OutHit.GetComponent())->GetName(), OutHit.Distance));

		//Snap to floor
		if (OutHit.Distance > 0) {
			FVector OwnerOrigin;
			FVector OwnerBounds;
			GetOwner()->GetActorBounds(true, OwnerOrigin, OwnerBounds);
			FVector NewLocationZ = FVector(CurrLocation.X, CurrLocation.Y, CurrLocation.Z - OutHit.Distance + OwnerBounds.Z);
			GetOwner()->SetActorLocation(NewLocationZ, false);
		}

		//Snap to center of component that was hit
		FVector UpdatedCurrLocation = GetOwner()->GetActorLocation();
		FVector HitComponentOrogin = OutHit.GetComponent()->Bounds.Origin;
		FVector NewLocationXY = FVector(HitComponentOrogin.X, HitComponentOrogin.Y, UpdatedCurrLocation.Z);
		GetOwner()->SetActorLocation(NewLocationXY, false);
	}
}



