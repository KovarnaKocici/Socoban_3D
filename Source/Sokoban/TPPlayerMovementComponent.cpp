// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPlayerMovementComponent.h"
#include "TPPawn.h"
#include "Sokoban.h"
#include "Block.h"
#include "Engine/World.h"

void UTPPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	if (IsLocked)
	{
		FVector CurrLocation = GetOwner()->GetActorLocation();
		UE_LOG(LogMovement, Log, TEXT("CurrLocation: %s  TargetLocation: %s"), *CurrLocation.ToString(), *TargetLocation.ToString());

		if (CurrLocation.Equals(TargetLocation, 0.005))
		{
			GetOwner()->SetActorLocation(TargetLocation);
			UE_LOG(LogMovement, Log, TEXT("Movement is done."));

			GetWorld()->GetTimerManager().SetTimer(InputTimeHandle, this, &UTPPlayerMovementComponent::EndTimer, 0.2f, false);
			IsLocked = false;

		}
		else 
		{
			// Get (and then clear) the movement vector that we set in ATPPawn::Tick
			FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, CurrAlpha);
			CurrAlpha += Alpha;
			GetOwner()->SetActorLocation(NewLocation);
		}
	}
}

void UTPPlayerMovementComponent::EndTimer() {
	
	ATPPawn* ThisPawn = Cast<ATPPawn>(GetOwner());
	if (ThisPawn) {
		ThisPawn->IsPushing = false;
		IsMoving = false;
	}
}

void UTPPlayerMovementComponent::SetAlpha(float Value) {
	if (0 < Value && Value <= 1)
		Alpha = Value;
}

float UTPPlayerMovementComponent::GetAlpha() {
	return Alpha;
}

float UTPPlayerMovementComponent::GetAxisValue() {
	return AxisValue;
}

void UTPPlayerMovementComponent::Move(FVector CurrMovement, float Axis, int CurrDirection) {
	IsLocked = true;
	IsMoving = true;
	UE_LOG(LogMovement, Log, TEXT("Movement is locked."));
	AxisValue = Axis;
	StartLocation = GetOwner()->GetActorLocation();
	TargetLocation = GetOwner()->GetActorLocation() + CurrMovement;
	//AddInputVector(Movement);
	CurrAlpha = Alpha;
	Direction = CurrDirection;
}

void UTPPlayerMovementComponent::ReverseMove() {
	TargetLocation = StartLocation;
	StartLocation = GetOwner()->GetActorLocation();
	CurrAlpha = Alpha;
}