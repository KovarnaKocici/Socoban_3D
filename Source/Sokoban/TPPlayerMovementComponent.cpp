// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPlayerMovementComponent.h"
#include "TPPawn.h"
#include "Engine/World.h"

void UTPPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	if (isLockedForward || isLockedRight)
	{
		CurrLocation = GetOwner()->GetActorLocation();
		TargetLocation = CurrLocation + FVector(ConsumeInputVector().GetClampedToMaxSize(1.0f) * 200);

		// Get (and then clear) the movement vector that we set in ATPPawn::Tick
		FVector CurrEnd;
		CurrEnd = FMath::Lerp(CurrLocation, TargetLocation, 0.1f);
		FVector  DesiredMovementThisFrame = CurrEnd - CurrLocation;
		if (!DesiredMovementThisFrame.IsNearlyZero())
		{
			FHitResult Hit;
			SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
			// If we bumped into something, try to slide along it
			if (Hit.IsValidBlockingHit())
			{
				SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
			}
		}
		//CurrLocation = CurrEnd;

		if (CurrLocation == TargetLocation)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, FString::Printf(TEXT("Movement is done")));
			if (isLockedForward) {
				isLockedRight = false; 				
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("isLockedRight = false")));
			}
			if (isLockedRight) {
				isLockedForward = false;
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("isLockedForward = false")));
			}
		}
	}
};
